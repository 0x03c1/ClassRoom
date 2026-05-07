#!/usr/bin/env python3
"""
Teste de Paralelismo - Aula 1 (Infraestrutura de Hardware)
Prof. Roni Maciel

Executa a mesma carga computacional (multiplicação de matrizes) em diferentes
quantidades de threads e gera uma tabela com tempos e fator de escala.

Uso: python3 teste_paralelismo.py
Dependências: numpy, psutil
"""

# IMPORTANTE: as variáveis abaixo DEVEM ser definidas antes de importar
# numpy/psutil. As bibliotecas BLAS (OpenBLAS, MKL, BLIS, Accelerate) leem
# essas variáveis no momento do import e ignoram alterações posteriores.
# Sem isso, o "single-thread" do teste roda multi-threaded e o speedup fica
# distorcido.
import os
os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("BLIS_NUM_THREADS", "1")
os.environ.setdefault("VECLIB_MAXIMUM_THREADS", "1")  # macOS

import multiprocessing as mp
import time
from concurrent.futures import ProcessPoolExecutor

import numpy as np
import psutil


TAMANHO_MATRIZ = 800   # ajuste para mais/menos carga
N_TAREFAS = 16         # tarefas a distribuir entre as threads


def carga_computacional(_):
    """Multiplicação de matrizes — boa para CPU, sem I/O."""
    a = np.random.rand(TAMANHO_MATRIZ, TAMANHO_MATRIZ)
    b = np.random.rand(TAMANHO_MATRIZ, TAMANHO_MATRIZ)
    c = a @ b
    return c.sum()


def medir(n_threads: int) -> float:
    inicio = time.perf_counter()
    with ProcessPoolExecutor(max_workers=n_threads) as executor:
        list(executor.map(carga_computacional, range(N_TAREFAS)))
    return time.perf_counter() - inicio


def main():
    nucleos_fisicos = psutil.cpu_count(logical=False)
    threads_logicas = psutil.cpu_count(logical=True)

    print("=" * 70)
    print("TESTE DE PARALELISMO — Lei de Amdahl na prática")
    print("=" * 70)
    print(f"Núcleos físicos:  {nucleos_fisicos}")
    print(f"Threads lógicas:  {threads_logicas}")
    print(f"Tarefas:          {N_TAREFAS} multiplicações de matrizes "
          f"{TAMANHO_MATRIZ}x{TAMANHO_MATRIZ}")
    print("BLAS limitado a 1 thread por processo (env vars setadas no topo)")
    print("=" * 70)

    configuracoes = [1, 2, 4, threads_logicas]
    configuracoes = sorted(set(c for c in configuracoes if c <= threads_logicas))

    print(f"\n{'Threads':<10}{'Tempo (s)':<15}{'Speedup':<12}"
          f"{'Eficiência (%)':<18}")
    print("-" * 55)

    tempo_base = None
    for n in configuracoes:
        t = medir(n)
        if tempo_base is None:
            tempo_base = t
            speedup = 1.0
        else:
            speedup = tempo_base / t
        eficiencia = (speedup / n) * 100
        print(f"{n:<10}{t:<15.2f}{speedup:<12.2f}{eficiencia:<18.1f}")

    print("\n" + "=" * 70)
    print("OBSERVAÇÃO PEDAGÓGICA")
    print("=" * 70)
    print("Speedup ideal seria igual ao número de threads (escala linear).")
    print("A queda de eficiência revela:")
    print("  • Overhead de criação/sincronização de processos")
    print("  • Disputa por cache L3 e largura de banda de RAM")
    print("  • Threads lógicas (SMT/HyperThreading) NÃO equivalem a núcleos reais")
    print("  • Lei de Amdahl: fração serial limita o ganho máximo")
    print("=" * 70)


if __name__ == "__main__":
    mp.set_start_method("spawn", force=True)
    main()
