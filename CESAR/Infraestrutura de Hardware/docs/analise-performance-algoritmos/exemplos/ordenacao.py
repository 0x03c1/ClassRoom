#!/usr/bin/env python3
"""
Exemplo de algoritmo para o comparador_stacks.py
Implementa quicksort manual em Python (NÃO usa o sort embutido para ser
comparável com C/Java).

Usa pivô por mediana-de-três para reduzir o pior caso (entrada já ordenada)
e definir um recursionlimit seguro mesmo para entradas patológicas.

Uso: python3 ordenacao.py <tamanho>
"""

import math
import random
import sys


def quicksort(arr, lo, hi):
    if lo < hi:
        # Mediana-de-três (lo, mid, hi) → coloca o pivô em arr[hi]
        mid = (lo + hi) // 2
        if arr[lo] > arr[mid]:
            arr[lo], arr[mid] = arr[mid], arr[lo]
        if arr[lo] > arr[hi]:
            arr[lo], arr[hi] = arr[hi], arr[lo]
        if arr[mid] > arr[hi]:
            arr[mid], arr[hi] = arr[hi], arr[mid]
        arr[mid], arr[hi] = arr[hi], arr[mid]

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

    # Margem segura para recursionlimit. Mediana-de-três torna O(log n) o caso
    # típico, mas reservamos espaço para o pior caso teórico.
    sys.setrecursionlimit(max(10_000, 4 * int(math.log2(max(n, 2))) + n))

    quicksort(arr, 0, n - 1)
    # Imprime apenas primeiro e último para validar
    print(f"{arr[0]} ... {arr[-1]}")


if __name__ == "__main__":
    main()
