#!/usr/bin/env python3
"""
Workload Completo - Aula 5 (Infraestrutura de Hardware)
Prof. Roni Maciel

Workload controlado com 3 fases distintas:
  1. CPU-bound (cálculo intensivo, dados pequenos — fica em cache)
  2. Memory-bound (varredura de buffer >>> L3 — sai do cache)
  3. I/O-bound (leitura/escrita em disco)

Coleta métricas em cada fase e infere o gargalo dominante da máquina.

Uso: python3 workload_completo.py

Nota: o número de "memory-bound bandwidth" obtido é uma SUBESTIMATIVA da
banda real de RAM, pois o NumPy usa SIMD e o controlador de memória
prefetcha agressivamente. Para um número mais fiel, use `mbw -t0 -n 5 1024`.
"""

import os
import tempfile
import time

import numpy as np
import psutil


def fase_cpu_bound(duracao_alvo: float = 8.0) -> dict:
    """Cálculo intensivo com dados pequenos — caberá em cache."""
    print("\n[1/3] Fase CPU-BOUND — multiplicação de matrizes pequenas")
    print("       (dados cabem em L2/L3, exercita pipeline e ALU)")

    inicio = time.perf_counter()
    iteracoes = 0

    while time.perf_counter() - inicio < duracao_alvo:
        a = np.random.rand(200, 200)
        b = np.random.rand(200, 200)
        _ = a @ b
        iteracoes += 1

    duracao = time.perf_counter() - inicio
    cpu_uso = psutil.cpu_percent(interval=None)

    return {
        "fase": "CPU-bound",
        "duracao_s": duracao,
        "iteracoes": iteracoes,
        "cpu_pct": cpu_uso,
        "throughput": iteracoes / duracao,
    }


def fase_memory_bound(duracao_alvo: float = 8.0) -> dict:
    """Varredura de buffer grande — sai do cache, estressa RAM."""
    print("\n[2/3] Fase MEMORY-BOUND — varredura de buffer 1 GB")
    print("       (sai de qualquer L3, exercita banda de RAM)")

    # 1 GB é maior que qualquer L3 atual (max ~128 MB em CPUs server)
    tamanho_mb = 1024
    n_elementos = (tamanho_mb * 1024 * 1024) // 8
    buffer = np.random.rand(n_elementos)

    # Toca todas as páginas para forçar commit antes de medir
    buffer[::4096] = 0.0

    inicio = time.perf_counter()
    iteracoes = 0
    soma = 0.0

    while time.perf_counter() - inicio < duracao_alvo:
        soma += float(np.sum(buffer))
        iteracoes += 1

    duracao = time.perf_counter() - inicio
    bytes_lidos = iteracoes * tamanho_mb * 1024 * 1024
    gb_por_s = bytes_lidos / duracao / (1024 ** 3)

    del buffer

    return {
        "fase": "Memory-bound",
        "duracao_s": duracao,
        "iteracoes": iteracoes,
        "bandwidth_gbs": gb_por_s,
    }


def fase_io_bound(duracao_alvo: float = 8.0) -> dict:
    """Escrita e leitura aleatória em disco temporário."""
    print("\n[3/3] Fase I/O-BOUND — leitura/escrita em disco")
    print("       (exercita SSD/HDD via PCIe ou SATA)")

    arquivo = tempfile.NamedTemporaryFile(delete=False, suffix=".bin")
    caminho = arquivo.name
    arquivo.close()

    bloco = os.urandom(4096)  # 4 KB random
    inicio = time.perf_counter()
    bytes_escritos = 0
    operacoes = 0

    try:
        with open(caminho, "wb") as f:
            while time.perf_counter() - inicio < duracao_alvo:
                f.write(bloco)
                bytes_escritos += len(bloco)
                operacoes += 1
                if operacoes % 100 == 0:
                    f.flush()
                    os.fsync(f.fileno())

        duracao = time.perf_counter() - inicio
        mb_por_s = (bytes_escritos / (1024 ** 2)) / duracao
    finally:
        try:
            os.unlink(caminho)
        except OSError:
            pass

    return {
        "fase": "I/O-bound",
        "duracao_s": duracao,
        "operacoes": operacoes,
        "throughput_mbs": mb_por_s,
    }


def inferir_gargalo(r_cpu, r_mem, r_io):
    """Heurística simples para apontar o gargalo dominante."""
    print("\n" + "=" * 70)
    print("INFERÊNCIA DE GARGALO")
    print("=" * 70)

    nucleos = psutil.cpu_count(logical=True) or 1
    pontos_cpu = r_cpu["throughput"]
    pontos_mem = r_mem["bandwidth_gbs"]
    pontos_io = r_io["throughput_mbs"]

    # Quantos núcleos estavam ocupados? Snapshot por núcleo.
    por_core = psutil.cpu_percent(interval=0.5, percpu=True)
    cores_usados = sum(1 for c in por_core if c > 50)

    print(f"  CPU:       {pontos_cpu:>10.1f} multiplicações/s")
    print(f"  Memória:   {pontos_mem:>10.2f} GB/s de varredura "
          "(subestimativa — ver mbw)")
    print(f"  I/O:       {pontos_io:>10.1f} MB/s de escrita")
    print(f"  Cores >50% no fim: {cores_usados}/{nucleos}")
    print()

    if pontos_io < 100:
        print("⚠ Disco lento (HDD?). Aplicações com muito I/O sofrerão.")
    elif pontos_io < 500:
        print("⚠ Provável SSD SATA. Um NVMe daria ganho considerável.")
    else:
        print("✓ Armazenamento rápido (provavelmente NVMe).")

    if pontos_mem < 5:
        print("⚠ Banda de RAM limitada. Verifique se está em dual-channel.")
    else:
        print("✓ RAM com banda saudável.")

    if cores_usados < max(2, nucleos // 2):
        print(f"⚠ Apenas {cores_usados}/{nucleos} núcleos saturados — este "
              "workload é single-thread. Há paralelismo a explorar.")
    else:
        print(f"✓ {cores_usados}/{nucleos} núcleos exercitados — workload "
              "bem paralelizado.")

    print("=" * 70)


def main():
    print("=" * 70)
    print("WORKLOAD COMPLETO — Profiling Integrado")
    print("=" * 70)
    print(f"CPU:    {psutil.cpu_count(logical=False)} núcleos / "
          f"{psutil.cpu_count(logical=True)} threads")
    ram_gb = psutil.virtual_memory().total / (1024 ** 3)
    print(f"RAM:    {ram_gb:.1f} GB")
    print(f"SO:     {os.name}")
    print("=" * 70)

    if ram_gb < 2.0:
        print("\n⚠ RAM total < 2 GB. A fase memory-bound pode falhar.")
        print("  Reduza o tamanho do buffer manualmente em fase_memory_bound.\n")

    r_cpu = fase_cpu_bound()
    r_mem = fase_memory_bound()
    r_io = fase_io_bound()

    print("\n" + "=" * 70)
    print("RESULTADOS POR FASE")
    print("=" * 70)
    for r in (r_cpu, r_mem, r_io):
        print(f"  {r}")

    inferir_gargalo(r_cpu, r_mem, r_io)

    print("\nPRÓXIMOS PASSOS:")
    print("  1. Compare seus números com uma execução anterior sua.")
    print("  2. Identifique a maior diferença e proponha uma explicação.")
    print("  3. Refine sua resposta às perguntas reflexivas do bloco 5.\n")


if __name__ == "__main__":
    main()
