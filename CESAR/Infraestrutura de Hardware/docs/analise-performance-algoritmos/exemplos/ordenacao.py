#!/usr/bin/env python3
"""
Exemplo de algoritmo para o comparador_stacks.py
Implementa quicksort manual em Python (NÃO usa o sort embutido para ser comparável com C/Java).

Uso: python3 ordenacao.py <tamanho>
"""

import random
import sys


def quicksort(arr, lo, hi):
    if lo < hi:
        pivot = arr[hi]
        i = lo - 1
        for j in range(lo, hi):
            if arr[j] <= pivot:
                i += 1
                arr[i], arr[j] = arr[j], arr[i]
        arr[i + 1], arr[hi] = arr[hi], arr[i + 1]
        p = i + 1
        quicksort(arr, lo, p - 1)
        quicksort(arr, p + 1, hi)


def main():
    if len(sys.argv) < 2:
        print("Uso: ordenacao.py <tamanho>", file=sys.stderr)
        sys.exit(1)
    n = int(sys.argv[1])
    random.seed(42)
    arr = [random.randint(0, 1_000_000) for _ in range(n)]
    sys.setrecursionlimit(max(1000, n + 100))
    quicksort(arr, 0, n - 1)
    # Imprime apenas o primeiro e último para validar (e ser consumido por /dev/null)
    print(f"{arr[0]} ... {arr[-1]}")


if __name__ == "__main__":
    main()
