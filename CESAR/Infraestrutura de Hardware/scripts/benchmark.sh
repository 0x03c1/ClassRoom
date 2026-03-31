#!/bin/bash
# ============================================================
# benchmark_completo.sh — Script mestre de benchmark
# Disciplina: Infraestrutura de Hardware (3º período)
#
# Este script compila e executa os 3 programas (ASM, C, Python)
# e gera um relatório comparativo no terminal.
#
# USO:
#   chmod +x benchmark_completo.sh
#   ./benchmark_completo.sh
#
# PRÉ-REQUISITOS:
#   - nasm (sudo apt install nasm)
#   - gcc  (sudo apt install gcc)
#   - python3
# ============================================================

set -e

echo ""
echo "╔══════════════════════════════════════════════════════════╗"
echo "║  BENCHMARK: Impacto da Linguagem no Hardware            ║"
echo "║  Infraestrutura de Hardware — 3º Período CC             ║"
echo "╚══════════════════════════════════════════════════════════╝"
echo ""

# --- Verificar dependências ---
for cmd in nasm gcc python3 objdump; do
    if ! command -v $cmd &> /dev/null; then
        echo "ERRO: $cmd não encontrado. Instale antes de continuar."
        exit 1
    fi
done

# --- Compilar ---
echo "[1/5] Compilando Assembly (NASM x86-64)..."
nasm -f elf64 soma_vetor.asm -o soma_vetor.o
ld soma_vetor.o -o soma_asm
echo "      Binário: $(ls -lh soma_asm | awk '{print $5}') | Seção .text: $(size soma_asm | tail -1 | awk '{print $1}') bytes"

echo ""
echo "[2/5] Compilando C (GCC -O0, -O2, -O3)..."
gcc -O0 soma_vetor.c -o soma_c_O0
gcc -O2 soma_vetor.c -o soma_c_O2
gcc -O3 soma_vetor.c -o soma_c_O3
echo "      C -O0: $(ls -lh soma_c_O0 | awk '{print $5}') | C -O2: $(ls -lh soma_c_O2 | awk '{print $5}') | C -O3: $(ls -lh soma_c_O3 | awk '{print $5}')"

echo ""
echo "[3/5] Gerando assembly do C (para inspeção)..."
gcc -O0 -S soma_vetor.c -o soma_O0.s
gcc -O3 -S soma_vetor.c -o soma_O3.s
echo "      Arquivos: soma_O0.s, soma_O3.s (compare com: diff soma_O0.s soma_O3.s)"

# --- Executar ---
echo ""
echo "┌──────────────────────────────────────────────────────────┐"
echo "│  EXECUTANDO BENCHMARKS (N = 1 bilhão, exceto Python)     │"
echo "└──────────────────────────────────────────────────────────┘"

echo ""
echo "[4/5] Executando binários nativos..."
echo ""
echo "--- Assembly puro ---"
start_asm=$(date +%s%N)
./soma_asm
end_asm=$(date +%s%N)
tempo_asm=$(( (end_asm - start_asm) / 1000000 ))
echo "Tempo total (incluindo I/O): ${tempo_asm} ms"

echo ""
echo "--- C -O0 (sem otimização) ---"
./soma_c_O0

echo ""
echo "--- C -O2 ---"
./soma_c_O2

echo ""
echo "--- C -O3 (máxima otimização) ---"
./soma_c_O3

echo ""
echo "[5/5] Executando Python..."
echo ""
python3 soma_vetor.py

# --- Análise extra ---
echo ""
echo "╔══════════════════════════════════════════════════════════╗"
echo "║  ANÁLISE DE INSTRUÇÕES (por iteração do loop)           ║"
echo "╠══════════════════════════════════════════════════════════╣"
echo "║  Assembly manual:  3 instr (ADD, DEC, JNZ)             ║"
echo "║  C -O0:            6 instr (2 MOV, ADD, ADD, CMP, JLE) ║"
echo "║  C -O3:            5 instr (MOV, ADD, ADD, CMP, JNE)   ║"
echo "║  Python (bytecde): ~8 opcodes + overhead de objetos    ║"
echo "╠══════════════════════════════════════════════════════════╣"
echo "║  TAMANHO DO LOOP NO BINÁRIO:                           ║"
echo "║  Assembly manual:  8 bytes (3 instruções)              ║"
echo "║  C -O3:           ~20 bytes (5 instruções)             ║"
echo "╚══════════════════════════════════════════════════════════╝"

echo ""
echo "┌──────────────────────────────────────────────────────────┐"
echo "│  COMANDOS ÚTEIS PARA EXPLORAR EM AULA:                  │"
echo "│                                                          │"
echo "│  objdump -d soma_asm          # disassembly do ASM      │"
echo "│  objdump -d soma_c_O3         # disassembly do C        │"
echo "│  diff soma_O0.s soma_O3.s     # ver efeito do otimizador│"
echo "│  python3 -m dis soma_vetor.py # bytecode Python         │"
echo "│  strace ./soma_asm            # syscalls do ASM         │"
echo "│  strace ./soma_c_O0           # syscalls do C (muito+)  │"
echo "│  gcc -O3 -S soma_vetor.c -fverbose-asm -o verbose.s    │"
echo "└──────────────────────────────────────────────────────────┘"
echo ""
