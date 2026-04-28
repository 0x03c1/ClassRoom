#!/usr/bin/env python3
"""
Big-O Visualizador - Caso 1 (Análise de Performance de Algoritmos)
Prof. Roni Maciel - Disciplina de Infraestrutura de Hardware

Roda algoritmos com tamanhos crescentes de entrada, mede o tempo,
infere a complexidade Big-O empírica e plota a curva log-log.

Uso: python3 big_o_visualizador.py
Dependências: pip install numpy matplotlib
"""

import random
import time
import bisect
import warnings
from typing import Callable, List, Tuple

import numpy as np
import matplotlib.pyplot as plt

warnings.filterwarnings("ignore", category=np.exceptions.RankWarning)


# ===== Algoritmos a serem testados =====

def soma_linear(arr):
    """O(n) — soma todos os elementos."""
    total = 0
    for x in arr:
        total += x
    return total


def busca_binaria(dados):
    """O(log n) — busca binária. Recebe (lista_ordenada, alvo)."""
    arr, alvo = dados
    if not arr:
        return -1
    return bisect.bisect_left(arr, alvo)


def bubble_sort(arr):
    """O(n²) — bubble sort clássico."""
    a = list(arr)
    n = len(a)
    for i in range(n):
        for j in range(0, n - i - 1):
            if a[j] > a[j + 1]:
                a[j], a[j + 1] = a[j + 1], a[j]
    return a


def timsort(arr):
    """O(n log n) — sort embutido do Python (Timsort)."""
    return sorted(arr)


# ===== Geradores de entrada =====

def gerador_lista_aleatoria(n):
    random.seed(42)
    return [random.randint(0, 1_000_000) for _ in range(n)]


def gerador_busca(n):
    n = max(n, 1)
    random.seed(42)
    arr = sorted(random.randint(0, 1_000_000) for _ in range(n))
    return (arr, random.choice(arr))


# ===== Configuração =====

ALGORITMOS = {
    "Soma linear":    (soma_linear,   gerador_lista_aleatoria, "O(n)"),
    "Busca binária":  (busca_binaria, gerador_busca,           "O(log n)"),
    "Bubble sort":    (bubble_sort,   gerador_lista_aleatoria, "O(n²)"),
    "Timsort":        (timsort,       gerador_lista_aleatoria, "O(n log n)"),
}


def medir_tempos(funcao: Callable, gerador: Callable,
                  tamanhos: List[int], repeticoes: int = 3) -> List[float]:
    """Mede tempo. Retorna mínimo de N repetições (reduz ruído de outros processos)."""
    tempos = []
    for n in tamanhos:
        entrada = gerador(n)
        melhores = []
        for _ in range(repeticoes):
            inicio = time.perf_counter()
            funcao(entrada)
            melhores.append(time.perf_counter() - inicio)
        tempos.append(min(melhores))
    return tempos


def inferir_complexidade(tamanhos: List[int], tempos: List[float]) -> Tuple[str, float]:
    """
    Inferência empírica de complexidade Big-O por melhor ajuste R².

    Para cada classe candidata, ajustamos um modelo t = a*f(n) + b
    e escolhemos a classe com maior coeficiente de determinação.

    Também retornamos a inclinação log-log para diagnóstico:
      - O(1):       inclinação ≈ 0
      - O(log n):   curva (não é reta perfeita)
      - O(n):       inclinação ≈ 1.0
      - O(n²):      inclinação ≈ 2.0
    """
    n_arr = np.array(tamanhos, dtype=float)
    t_arr = np.array(tempos, dtype=float)

    # Inclinação em log-log para diagnóstico
    log_n = np.log(n_arr)
    log_t = np.log(t_arr)
    inclinacao = np.polyfit(log_n, log_t, 1)[0]

    candidatos = {
        "O(1)":       np.ones_like(n_arr),
        "O(log n)":   np.log(n_arr),
        "O(n)":       n_arr,
        "O(n log n)": n_arr * np.log(n_arr),
        "O(n²)":      n_arr ** 2,
        "O(n³)":      n_arr ** 3,
    }

    melhor_classe = None
    melhor_r2 = -np.inf

    for nome, x in candidatos.items():
        a, b = np.polyfit(x, t_arr, 1)
        previsto = a * x + b
        ss_res = np.sum((t_arr - previsto) ** 2)
        ss_tot = np.sum((t_arr - np.mean(t_arr)) ** 2)
        r2 = 1 - ss_res / ss_tot if ss_tot > 0 else 0
        if r2 > melhor_r2:
            melhor_r2 = r2
            melhor_classe = nome

    return melhor_classe, inclinacao


def main():
    print("=" * 75)
    print("BIG-O VISUALIZADOR — Complexidade Empírica de Algoritmos")
    print("=" * 75)
    print()
    print("Para cada algoritmo:")
    print("  1. Rodamos com tamanhos crescentes de entrada")
    print("  2. Medimos o tempo (mínimo de 3 repetições, reduz ruído)")
    print("  3. Ajustamos modelos teóricos e escolhemos o de melhor R²")
    print()

    print(f"{'Algoritmo':<18}{'Inferida':<14}{'Inclinação':<14}{'Esperada':<14}{'Bate?':<6}")
    print("-" * 75)

    fig, ax = plt.subplots(figsize=(11, 7))

    for nome, (funcao, gerador, esperada) in ALGORITMOS.items():
        if "Bubble" in nome:
            tamanhos = [100, 250, 500, 1000, 1500, 2000, 3000]
        elif "Busca" in nome:
            tamanhos = [1000, 5000, 10000, 50000, 100000, 500000, 1000000]
        else:
            tamanhos = [1000, 5000, 10000, 50000, 100000, 250000, 500000]

        tempos = medir_tempos(funcao, gerador, tamanhos)
        inferida, inclin = inferir_complexidade(tamanhos, tempos)

        bate = "✓" if inferida == esperada else "?"
        print(f"{nome:<18}{inferida:<14}{inclin:<14.3f}{esperada:<14}{bate:<6}")

        ax.loglog(tamanhos, tempos, "o-",
                  label=f"{nome} → {inferida} (esperado {esperada})",
                  linewidth=2, markersize=8)

    ax.set_xlabel("Tamanho da entrada (n)", fontsize=12)
    ax.set_ylabel("Tempo de execução (s)", fontsize=12)
    ax.set_title("Complexidade Empírica de Algoritmos (escala log-log)", fontsize=13)
    ax.grid(True, which="both", linestyle="--", alpha=0.5)
    ax.legend(loc="upper left", fontsize=9)

    plt.tight_layout()
    arquivo_saida = "complexidade_empirica.png"
    plt.savefig(arquivo_saida, dpi=120)
    print()
    print("=" * 75)
    print(f"✓ Gráfico salvo em: {arquivo_saida}")
    print("=" * 75)
    print()
    print("COMO LER O GRÁFICO LOG-LOG:")
    print("  • A inclinação da reta indica a classe de complexidade.")
    print("  • Reta horizontal      → O(1)")
    print("  • Inclinação suave     → O(log n)")
    print("  • Inclinação a 45°     → O(n)")
    print("  • Inclinação maior     → O(n²), O(n³), ...")
    print()
    print("EXERCÍCIO: troque um dos algoritmos acima pelo SEU algoritmo")
    print("           e veja se a curva empírica bate com a teoria.")
    print("=" * 75)


if __name__ == "__main__":
    main()
