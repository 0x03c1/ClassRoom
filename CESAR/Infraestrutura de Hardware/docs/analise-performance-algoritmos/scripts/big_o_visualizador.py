#!/usr/bin/env python3
"""
Big-O Visualizador - Caso 1 (Análise de Performance de Algoritmos)
Prof. Roni Maciel - Disciplina de Infraestrutura de Hardware

Roda algoritmos com tamanhos crescentes de entrada, mede o tempo,
infere a complexidade Big-O empírica e plota a curva log-log.

A inferência usa a INCLINAÇÃO da reta em log-log como classificador
primário, e R² apenas como desempate entre classes próximas.
Esse método é robusto onde o R² puro falha (especialmente para O(log n),
em que tempos pequenos são dominados pelo ruído de medição).

Uso: python3 big_o_visualizador.py
Dependências: pip install numpy matplotlib
"""

import bisect
import math
import random
import time
import warnings
from typing import Callable, List, Tuple

import matplotlib.pyplot as plt
import numpy as np

warnings.filterwarnings("ignore", category=np.exceptions.RankWarning)


# ===== Algoritmos a serem testados =====

def soma_linear(arr):
    """O(n) — soma todos os elementos."""
    total = 0
    for x in arr:
        total += x
    return total


def busca_binaria(dados):
    """O(log n) — busca binária. Recebe (lista_ordenada, alvos).
    Faz muitas buscas para acumular tempo mensurável."""
    arr, alvos = dados
    if not arr:
        return -1
    last = -1
    for alvo in alvos:
        last = bisect.bisect_left(arr, alvo)
    return last


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
    """Gera lista ordenada de tamanho n e 100k alvos para busca repetida.

    O número de alvos é constante para que o tempo total cresça com log(n)
    e a inclinação log-log fique limpa.
    """
    n = max(n, 1)
    random.seed(42)
    arr = sorted(random.randint(0, 1_000_000) for _ in range(n))
    alvos = [random.choice(arr) for _ in range(100_000)]
    return (arr, alvos)


# ===== Configuração =====

ALGORITMOS = {
    "Soma linear":   (soma_linear,   gerador_lista_aleatoria, "O(n)"),
    "Busca binária": (busca_binaria, gerador_busca,           "O(log n)"),
    "Bubble sort":   (bubble_sort,   gerador_lista_aleatoria, "O(n²)"),
    "Timsort":       (timsort,       gerador_lista_aleatoria, "O(n log n)"),
}


def medir_tempos(funcao: Callable, gerador: Callable,
                 tamanhos: List[int], repeticoes: int = 3) -> List[float]:
    """Mede tempo. Retorna mínimo de N repetições (reduz ruído)."""
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


def inferir_complexidade(tamanhos: List[int],
                         tempos: List[float]) -> Tuple[str, float]:
    """
    Inferência empírica de complexidade Big-O em duas etapas:

      1. Calcula a inclinação log-log (slope) — esse é o melhor indicador
         de classe assintótica.
      2. Mapeia slope para classe por faixa, usando R² para desempate
         entre classes que têm a mesma inclinação dominante (ex: O(n) e
         O(n log n) são ambas próximas de slope=1).

    Faixas (calibradas empiricamente em Python 3.x):
      slope < 0.5    → O(1) ou O(log n)    (decide por R²)
      0.5 ≤ s < 1.3  → O(n) ou O(n log n)  (decide por R²)
      1.3 ≤ s < 2.5  → O(n²)
      s ≥ 2.5        → O(n³) ou pior

    Obs: a faixa de O(log n) é generosa porque, em Python interpretado,
    o overhead do laço externo (constante por iteração) costuma somar
    junto com o log(n) e empurra a inclinação para cima.
    """
    n_arr = np.array(tamanhos, dtype=float)
    t_arr = np.array(tempos, dtype=float)

    # Tempos muito pequenos = puro ruído. Avisa em vez de mentir.
    if t_arr.max() < 1e-4:
        return "ruído (medição < 0.1ms — aumente n)", 0.0

    log_n = np.log(n_arr)
    log_t = np.log(np.maximum(t_arr, 1e-9))
    slope = float(np.polyfit(log_n, log_t, 1)[0])

    # Candidatos para desempate
    candidatos_log = {
        "O(1)":     np.ones_like(n_arr),
        "O(log n)": np.log(n_arr),
    }
    candidatos_lin = {
        "O(n)":       n_arr,
        "O(n log n)": n_arr * np.log(n_arr),
    }

    def melhor_r2(candidatos):
        melhor, melhor_r = None, -np.inf
        for nome, x in candidatos.items():
            a, b = np.polyfit(x, t_arr, 1)
            previsto = a * x + b
            ss_res = np.sum((t_arr - previsto) ** 2)
            ss_tot = np.sum((t_arr - np.mean(t_arr)) ** 2)
            r2 = 1 - ss_res / ss_tot if ss_tot > 0 else 0
            if r2 > melhor_r:
                melhor_r, melhor = r2, nome
        return melhor

    if slope < 0.5:
        classe = melhor_r2(candidatos_log)
    elif slope < 1.3:
        classe = melhor_r2(candidatos_lin)
    elif slope < 2.5:
        classe = "O(n²)"
    else:
        classe = "O(n³)"

    return classe, slope


def main():
    print("=" * 75)
    print("BIG-O VISUALIZADOR — Complexidade Empírica de Algoritmos")
    print("=" * 75)
    print()
    print("Para cada algoritmo:")
    print("  1. Rodamos com tamanhos crescentes de entrada")
    print("  2. Medimos o tempo (mínimo de 3 repetições, reduz ruído)")
    print("  3. Calculamos a inclinação log-log e mapeamos para classe")
    print()

    print(f"{'Algoritmo':<18}{'Inferida':<22}{'Inclinação':<14}"
          f"{'Esperada':<14}{'Bate?':<6}")
    print("-" * 75)

    fig, ax = plt.subplots(figsize=(11, 7))

    for nome, (funcao, gerador, esperada) in ALGORITMOS.items():
        if "Bubble" in nome:
            tamanhos = [100, 250, 500, 1000, 1500, 2000, 3000]
        elif "Busca" in nome:
            # Cada execução faz 100.000 buscas no array. Com isso, tempos
            # ficam mensuráveis e a inclinação log-log reflete O(log n).
            tamanhos = [1_000, 10_000, 100_000, 1_000_000, 10_000_000]
        else:
            tamanhos = [1000, 5000, 10_000, 50_000, 100_000, 250_000, 500_000]

        tempos = medir_tempos(funcao, gerador, tamanhos)
        inferida, inclin = inferir_complexidade(tamanhos, tempos)

        bate = "✓" if inferida == esperada else "?"
        print(f"{nome:<18}{inferida:<22}{inclin:<14.3f}"
              f"{esperada:<14}{bate:<6}")

        ax.loglog(tamanhos, tempos, "o-",
                  label=f"{nome} → {inferida} (esperado {esperada})",
                  linewidth=2, markersize=8)

    ax.set_xlabel("Tamanho da entrada (n)", fontsize=12)
    ax.set_ylabel("Tempo de execução (s)", fontsize=12)
    ax.set_title("Complexidade Empírica de Algoritmos (escala log-log)",
                 fontsize=13)
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
    print("DICA: se o tempo medido for menor que ~0.1ms, o ruído domina")
    print("      e a inferência fica instável. Aumente n até o tempo")
    print("      ficar acima de 10ms para resultados confiáveis.")
    print()
    print("EXERCÍCIO: troque um dos algoritmos acima pelo SEU algoritmo")
    print("           e veja se a curva empírica bate com a teoria.")
    print("=" * 75)


if __name__ == "__main__":
    main()
