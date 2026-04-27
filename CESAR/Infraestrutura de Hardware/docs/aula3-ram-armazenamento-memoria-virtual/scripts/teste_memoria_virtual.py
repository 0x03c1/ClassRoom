#!/usr/bin/env python3
"""
Teste de Memória Virtual - Aula 3 (Infraestrutura de Hardware)
Prof. Ronierison Maciel

Aloca buffers progressivamente maiores e mede o tempo de acesso aleatório.
Quando o tamanho do buffer ultrapassa a RAM disponível, o sistema começa
a paginar para o swap — e o tempo de acesso explode.

Uso: python3 teste_memoria_virtual.py
ATENÇÃO: este script PODE deixar a máquina lenta temporariamente.
         Feche trabalhos importantes antes de executar.
"""

import time
import psutil
import numpy as np


def medir_acesso_aleatorio(tamanho_mb: int, n_acessos: int = 200_000) -> float:
    """Aloca um buffer de N MB e faz acessos aleatórios, medindo o tempo."""
    n_elementos = (tamanho_mb * 1024 * 1024) // 8  # int64 = 8 bytes
    buffer = np.arange(n_elementos, dtype=np.int64)

    # Toca o buffer inteiro para forçar alocação real (commit das páginas)
    buffer[::4096] = 1

    indices = np.random.randint(0, n_elementos, size=n_acessos)

    inicio = time.perf_counter()
    total = 0
    for idx in indices:
        total += int(buffer[idx])
    duracao = time.perf_counter() - inicio

    del buffer
    return duracao


def main():
    ram_total_mb = psutil.virtual_memory().total // (1024 * 1024)
    ram_disponivel_mb = psutil.virtual_memory().available // (1024 * 1024)

    print("=" * 70)
    print("TESTE DE MEMÓRIA VIRTUAL — Inflexão RAM → Swap")
    print("=" * 70)
    print(f"RAM total:          {ram_total_mb:>8} MB")
    print(f"RAM disponível:     {ram_disponivel_mb:>8} MB")
    print()
    print("Vamos alocar buffers crescentes e medir o tempo de acesso aleatório.")
    print("Enquanto o buffer cabe na RAM, o tempo cresce devagar.")
    print("Quando ultrapassa, o tempo EXPLODE (paginação para o disco).")
    print("=" * 70)
    print()

    # Tamanhos de teste — ajuste se sua RAM for muito pequena ou muito grande
    tamanhos_mb = [
        64, 256, 1024,
        ram_disponivel_mb // 2,
        int(ram_disponivel_mb * 0.75),
        int(ram_disponivel_mb * 0.9),
    ]
    # Remove duplicatas e ordena
    tamanhos_mb = sorted(set(t for t in tamanhos_mb if 64 <= t <= ram_total_mb))

    print(f"{'Tamanho (MB)':<15}{'% RAM total':<15}{'Tempo (s)':<15}{'µs/acesso':<15}")
    print("-" * 60)

    for tam in tamanhos_mb:
        try:
            tempo = medir_acesso_aleatorio(tam)
            us_por_acesso = (tempo / 200_000) * 1_000_000
            pct = (tam / ram_total_mb) * 100
            print(f"{tam:<15}{pct:<15.1f}{tempo:<15.3f}{us_por_acesso:<15.2f}")
        except MemoryError:
            print(f"{tam:<15}MEMÓRIA INSUFICIENTE — pulando")
            break

    print()
    print("=" * 70)
    print("INTERPRETAÇÃO")
    print("=" * 70)
    print("• Buffers pequenos → cabem em cache L2/L3 → acesso muito rápido.")
    print("• Buffers médios → cabem na RAM → acesso na ordem de ~100 ns.")
    print("• Buffers grandes (>RAM) → paginação para SSD/HDD → 10.000x mais lento.")
    print()
    print("É por isso que 'falta de RAM' destrói o desempenho de bancos de dados,")
    print("máquinas virtuais e servidores de aplicação.")
    print("=" * 70)


if __name__ == "__main__":
    main()
