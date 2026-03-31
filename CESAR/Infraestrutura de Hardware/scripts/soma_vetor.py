import time
import dis
import sys

N = 1_000_000_000  # 1 bilhão (mesma escala que ASM e C)

N_PYTHON_LOOP = 100_000_000  # 100 milhões

print(f"{'='*60}")
print(f"  BENCHMARK: Soma de inteiros")
print(f"  Python {sys.version.split()[0]} | Plataforma: {sys.platform}")
print(f"{'='*60}")

# --- Loop puro ---
print(f"\n[1] Loop puro Python (N = {N_PYTHON_LOOP:,}) ...")
t1 = time.perf_counter()

soma = 0
for i in range(1, N_PYTHON_LOOP + 1):
    soma += i

t2 = time.perf_counter()
tempo_loop = (t2 - t1) * 1000  # ms

print(f"    Soma  = {soma}")
print(f"    Tempo = {tempo_loop:.2f} ms")

print(f"\n[2] sum(range(...)) — built-in em C (N = {N:,}) ...")
t1 = time.perf_counter()

soma_builtin = sum(range(1, N + 1))

t2 = time.perf_counter()
tempo_builtin = (t2 - t1) * 1000

print(f"    Soma  = {soma_builtin}")
print(f"    Tempo = {tempo_builtin:.2f} ms")

print(f"\n[3] Fórmula de Gauss — O(1) (N = {N:,}) ...")
t1 = time.perf_counter()

soma_gauss = N * (N + 1) // 2

t2 = time.perf_counter()
tempo_gauss = (t2 - t1) * 1000

print(f"    Soma  = {soma_gauss}")
print(f"    Tempo = {tempo_gauss:.4f} ms")

print(f"\n{'='*60}")
print(f"  BYTECODE do loop Python (dis.dis):")
print(f"{'='*60}")


def soma_simples():
    """Versão simplificada para visualizar bytecode."""
    s = 0
    for i in range(10):
        s += i
    return s


dis.dis(soma_simples)

print(f"\n{'='*60}")
print(f"  RESUMO COMPARATIVO")
print(f"{'='*60}")
print(f"  Loop Python ({N_PYTHON_LOOP/1e6:.0f}M): {tempo_loop:>10.2f} ms")
print(f"  sum() built-in ({N/1e6:.0f}M):  {tempo_builtin:>10.2f} ms")
print(f"  Gauss O(1):               {tempo_gauss:>10.4f} ms")
if tempo_builtin > 0:
    ratio = tempo_loop / tempo_builtin * (N / N_PYTHON_LOOP)
    print(f"\n  Loop puro é ~{ratio:.0f}x mais lento que sum() (escala ajustada)")
print(f"{'='*60}")
