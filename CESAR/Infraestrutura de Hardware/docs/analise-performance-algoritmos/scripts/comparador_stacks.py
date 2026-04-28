#!/usr/bin/env python3
"""
Comparador de Stacks - Caso 3 (Análise de Performance de Algoritmos)
Prof. Roni Maciel - Disciplina de Infraestrutura de Hardware

Roda múltiplos comandos (binários, scripts Python, JARs Java, etc.) com
tamanhos crescentes de entrada e gera gráfico comparativo de performance.

Cada comando deve aceitar o tamanho da entrada como ARGUMENTO de linha de comando
e imprimir apenas o tempo (em segundos) ou apenas o resultado — o tempo total é
medido externamente pelo wrapper.

Uso básico:
    python3 comparador_stacks.py \\
        --tamanhos 1000 5000 10000 \\
        --repeticoes 5 \\
        --comandos "python3 ord.py" "./ord_c" "java -cp . Ordenacao"

Saídas:
    - resultados.csv     (medições brutas)
    - comparacao_stacks.png (gráfico de linhas)

Dependências: pip install matplotlib psutil
"""

import argparse
import csv
import shlex
import subprocess
import time
from typing import Dict

import matplotlib.pyplot as plt
import psutil


def medir_execucao(comando: str, tamanho: int, timeout: int = 120) -> Dict:
    """
    Executa o comando passando 'tamanho' como argumento e mede o tempo + memória.
    Retorna dict com tempo, pico de memória, código de saída.
    """
    args = shlex.split(comando) + [str(tamanho)]

    inicio = time.perf_counter()
    pico_mem = 0
    try:
        proc = subprocess.Popen(args, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        ps_proc = psutil.Process(proc.pid)

        # Sample de memória durante a execução
        while proc.poll() is None:
            try:
                mem = ps_proc.memory_info().rss / (1024 * 1024)
                pico_mem = max(pico_mem, mem)
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                break
            if time.perf_counter() - inicio > timeout:
                proc.kill()
                return {"tempo": float("inf"), "memoria_mb": 0, "ok": False, "erro": "timeout"}
            time.sleep(0.01)

        proc.wait(timeout=5)
        tempo = time.perf_counter() - inicio
        ok = proc.returncode == 0
        return {"tempo": tempo, "memoria_mb": pico_mem, "ok": ok,
                "erro": "" if ok else f"exit code {proc.returncode}"}
    except FileNotFoundError:
        return {"tempo": float("inf"), "memoria_mb": 0, "ok": False,
                "erro": f"comando não encontrado: {args[0]}"}
    except Exception as e:
        return {"tempo": float("inf"), "memoria_mb": 0, "ok": False, "erro": str(e)}


def main():
    parser = argparse.ArgumentParser(
        description="Compara performance de múltiplas stacks no mesmo algoritmo.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument("--tamanhos", nargs="+", type=int, required=True,
                        help="Tamanhos de entrada (ex: 1000 5000 10000)")
    parser.add_argument("--repeticoes", type=int, default=3,
                        help="Repetições por tamanho (padrão: 3, mantém o mínimo)")
    parser.add_argument("--comandos", nargs="+", required=True,
                        help='Comandos a executar (use aspas: "python3 script.py")')
    parser.add_argument("--saida-csv", default="resultados.csv")
    parser.add_argument("--saida-png", default="comparacao_stacks.png")
    parser.add_argument("--timeout", type=int, default=120)
    args = parser.parse_args()

    print("=" * 75)
    print("COMPARADOR DE STACKS — Performance Multi-Linguagem / Multi-Máquina")
    print("=" * 75)
    print(f"Comandos:    {args.comandos}")
    print(f"Tamanhos:    {args.tamanhos}")
    print(f"Repetições:  {args.repeticoes} (mantém o mínimo de cada conjunto)")
    print(f"Máquina:     {psutil.cpu_count(logical=False)} cores físicos / "
          f"{psutil.virtual_memory().total // (1024**3)} GB RAM")
    print("=" * 75)
    print()

    resultados = []
    for cmd in args.comandos:
        print(f"\n▶ Executando: {cmd}")
        for n in args.tamanhos:
            tempos = []
            mems = []
            erro_final = ""
            for r in range(args.repeticoes):
                resultado = medir_execucao(cmd, n, timeout=args.timeout)
                if resultado["ok"]:
                    tempos.append(resultado["tempo"])
                    mems.append(resultado["memoria_mb"])
                else:
                    erro_final = resultado["erro"]

            if tempos:
                tempo_min = min(tempos)
                mem_max = max(mems) if mems else 0
                resultados.append({
                    "comando": cmd, "tamanho": n,
                    "tempo_s": tempo_min, "memoria_mb": mem_max, "ok": True,
                })
                print(f"  n={n:>8}: {tempo_min:>8.4f}s  ({mem_max:>6.1f} MB de pico)")
            else:
                resultados.append({
                    "comando": cmd, "tamanho": n,
                    "tempo_s": None, "memoria_mb": None, "ok": False, "erro": erro_final,
                })
                print(f"  n={n:>8}: FALHOU — {erro_final}")

    # ===== CSV =====
    with open(args.saida_csv, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=["comando", "tamanho", "tempo_s", "memoria_mb", "ok"])
        writer.writeheader()
        for r in resultados:
            writer.writerow({k: r.get(k, "") for k in writer.fieldnames})
    print(f"\n✓ CSV salvo: {args.saida_csv}")

    # ===== Gráfico =====
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

    for cmd in args.comandos:
        pts = [(r["tamanho"], r["tempo_s"]) for r in resultados
               if r["comando"] == cmd and r["ok"]]
        if pts:
            xs, ys = zip(*pts)
            ax1.plot(xs, ys, "o-", label=cmd, linewidth=2, markersize=8)

        pts_mem = [(r["tamanho"], r["memoria_mb"]) for r in resultados
                   if r["comando"] == cmd and r["ok"]]
        if pts_mem:
            xs, ys = zip(*pts_mem)
            ax2.plot(xs, ys, "s--", label=cmd, linewidth=2, markersize=7)

    ax1.set_xlabel("Tamanho da entrada (n)", fontsize=11)
    ax1.set_ylabel("Tempo (s)", fontsize=11)
    ax1.set_title("Tempo de execução", fontsize=12)
    ax1.set_xscale("log")
    ax1.set_yscale("log")
    ax1.grid(True, which="both", linestyle="--", alpha=0.5)
    ax1.legend(fontsize=9)

    ax2.set_xlabel("Tamanho da entrada (n)", fontsize=11)
    ax2.set_ylabel("Pico de memória (MB)", fontsize=11)
    ax2.set_title("Uso de memória", fontsize=12)
    ax2.set_xscale("log")
    ax2.grid(True, which="both", linestyle="--", alpha=0.5)
    ax2.legend(fontsize=9)

    plt.suptitle("Comparação de Stacks — Performance e Memória", fontsize=14)
    plt.tight_layout()
    plt.savefig(args.saida_png, dpi=120)
    print(f"✓ Gráfico salvo: {args.saida_png}")

    # ===== Tabela final de razões =====
    print()
    print("=" * 75)
    print("RAZÕES DE PERFORMANCE (relativas ao mais rápido em cada tamanho)")
    print("=" * 75)
    cabecalho = f"{'Tamanho':<12}" + "".join(f"{c[:25]:<27}" for c in args.comandos)
    print(cabecalho)
    print("-" * len(cabecalho))
    for n in args.tamanhos:
        tempos_n = {r["comando"]: r["tempo_s"] for r in resultados
                    if r["tamanho"] == n and r["ok"]}
        if not tempos_n:
            continue
        mais_rapido = min(tempos_n.values())
        linha = f"{n:<12}"
        for cmd in args.comandos:
            t = tempos_n.get(cmd)
            if t is None:
                linha += f"{'-':<27}"
            else:
                razao = t / mais_rapido
                marca = " (mais rápido)" if razao == 1.0 else f" ({razao:.1f}x)"
                linha += f"{t:.4f}s{marca:<19}"
        print(linha)
    print("=" * 75)


if __name__ == "__main__":
    main()
