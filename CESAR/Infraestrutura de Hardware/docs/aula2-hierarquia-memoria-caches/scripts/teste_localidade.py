#!/usr/bin/env python3
"""
Teste de Localidade de Cache - Aula 2 (Infraestrutura de Hardware)
Prof. Roni Maciel

Demonstra empiricamente o impacto da localidade espacial:
- Loop A: percorre matriz linha por linha (cache-friendly)
- Loop B: percorre a mesma matriz coluna por coluna (cache-hostile)

Mesma quantidade de operações, tempos drasticamente diferentes.

Uso: python3 teste_localidade.py
"""

import time
import numpy as np


# Matriz grande o suficiente para sair do L3 (~ 256 MB com float64)
N = 6000


def somar_por_linhas(matriz: np.ndarray) -> float:
    """Acesso sequencial: percorre linha por linha — amigo do cache."""
    total = 0.0
    for i in range(matriz.shape[0]):
        for j in range(matriz.shape[1]):
            total += matriz[i, j]
    return total


def somar_por_colunas(matriz: np.ndarray) -> float:
    """Acesso 'saltando': percorre coluna por coluna — hostil ao cache."""
    total = 0.0
    for j in range(matriz.shape[1]):
        for i in range(matriz.shape[0]):
            total += matriz[i, j]
    return total


def medir(funcao, matriz):
    inicio = time.perf_counter()
    funcao(matriz)
    return time.perf_counter() - inicio


def main():
    print("=" * 70)
    print("TESTE DE LOCALIDADE DE CACHE — Localidade Espacial na prática")
    print("=" * 70)
    print(f"Matriz: {N}x{N} elementos float64")
    print(f"Tamanho em memória: {(N * N * 8) / (1024 ** 2):.1f} MB")
    print(f"Total de operações por loop: {N*N:,}")
    print("=" * 70)
    print()
    print(" Para o efeito ser visível, este script usa loops Python puros.")
    print("  Com NumPy vetorizado, o compilador resolveria isso pra nós.")
    print("  Reduza N se a sua máquina demorar muito (sugestão: 2000).")
    print()

    # Cria a matriz com valores aleatórios
    print("Criando matriz...")
    matriz = np.random.rand(N, N)

    # Aquecimento (warm-up) para evitar viés do primeiro acesso
    print("Aquecimento...")
    _ = matriz.sum()

    # Teste A: linha por linha
    print("\n[A] Percorrendo linha por linha (sequencial)...")
    tempo_a = medir(somar_por_linhas, matriz)
    print(f"    Tempo: {tempo_a:.2f}s")

    # Teste B: coluna por coluna
    print("\n[B] Percorrendo coluna por coluna (com saltos na memória)...")
    tempo_b = medir(somar_por_colunas, matriz)
    print(f"    Tempo: {tempo_b:.2f}s")

    razao = tempo_b / tempo_a

    print()
    print("=" * 70)
    print("RESULTADO")
    print("=" * 70)
    print(f"Loop A (sequencial):   {tempo_a:>8.2f}s")
    print(f"Loop B (com saltos):   {tempo_b:>8.2f}s")
    print(f"Razão B/A:             {razao:>8.2f}x  ← acesso ruim é {razao:.1f}x mais lento")
    print()
    print("INTERPRETAÇÃO:")
    print("  • A matriz é armazenada em memória 'row-major' (linhas contíguas).")
    print("  • Loop A acessa endereços vizinhos → o cache traz os próximos")
    print("    elementos de graça (cache hit).")
    print("  • Loop B salta de linha em linha → o cache é invalidado a cada")
    print("    acesso (cache miss).")
    print("  • Mesma quantidade de cálculo, MUITA diferença de tempo.")
    print("  • Conclusão: o gargalo não é a CPU — é trazer dados até ela.")
    print("=" * 70)


if __name__ == "__main__":
    main()
