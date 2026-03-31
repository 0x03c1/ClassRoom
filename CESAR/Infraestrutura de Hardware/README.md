# BENCHMARK DE LINGUAGENS — Impacto Real no Hardware

## 1. Motivação

Quando escrevemos `soma += i` em Python, C ou Assembly, o resultado matemático é idêntico. Mas o **custo para o hardware** é radicalmente diferente. Este material demonstra, com números reais, o impacto que a escolha da linguagem tem sobre: ciclos de CPU, acessos à memória, tamanho do binário e tempo de execução.

---

## 2. O Experimento

**Tarefa:** somar todos os inteiros de 1 até N (N = 1 bilhão). O mesmo algoritmo, implementado em três níveis de abstração:

1. **Assembly x86-64 (NASM)** — instruções diretas para a CPU, zero abstração
2. **C (GCC)** — compilado para código nativo, com otimizações do compilador
3. **Python 3 (CPython)** — interpretado, tipagem dinâmica, garbage collector

---

## 3. Resultados Medidos

Todos os testes executados na mesma máquina, mesma carga, resultado verificado: **500.000.000.500.000.000**

| Linguagem | N | Tempo | Instr/iter | Binário |
|---|---|---|---|---|
| **Assembly (NASM)** | 1 bi | **~400 ms** ✅ | 3 | 8.9 KB |
| **C (GCC -O0)** | 1 bi | ~850 ms | 6 | 16 KB |
| **C (GCC -O3)** | 1 bi | **~420 ms** ✅ | 5 | 16 KB |
| **Python (loop)** | 100 mi* | **~7.450 ms** ❌ | ~8 opcodes | N/A |
| **Python sum()** | 1 bi | ~13.500 ms | C interno | N/A |
| **Python Gauss** | 1 bi | **~0.005 ms** ✅ | O(1) | N/A |

> \* Python loop puro usa N=100M (1 bilhão levaria ~75 segundos). Tempo extrapolado para 1 bi: ~74.500 ms.

### 3.1 Fator de Diferença

| Comparação | Fator | Analogia |
|---|---|---|
| Python loop vs ASM | **~185x mais lento** | Caminhar vs. carro a 180 km/h |
| Python loop vs C -O3 | **~177x mais lento** | Escada vs. elevador expresso |
| C -O0 vs C -O3 | ~2x mais lento | Estrada com curvas vs. reta |
| C -O3 vs ASM manual | **~1.05x (quase igual!)** | O compilador quase iguala o humano |

---

## 4. Por Que Python é Lento Neste Cenário?

### 4.1 O Custo de Cada Operação

Em Assembly, `soma += i` é **UMA** instrução (`ADD`). Em Python, a mesma operação dispara uma cadeia de eventos:

1. `LOAD_FAST` — busca a variável `soma` na tabela de locais (lookup em dict)
2. `LOAD_FAST` — busca `i`
3. `BINARY_ADD` — verifica tipo de ambos operandos, resolve `__add__`, aloca NOVO objeto int na heap (~28 bytes)
4. `STORE_FAST` — armazena resultado, decrementa refcount do antigo, pode disparar GC
5. Gerenciamento do iterador `range()` + branch prediction do interpretador

### 4.2 Representando um Inteiro

| Linguagem | Onde vive o inteiro | Tamanho |
|---|---|---|
| **Assembly** | Registrador (RAX, RCX...) | 8 bytes (fixo) ✅ |
| **C** | Stack ou registrador | 8 bytes (int64_t) ✅ |
| **Python** | Objeto na HEAP (malloc) | ~28 bytes + overhead ❌ |

Cada inteiro em Python é um **objeto completo** com: header (tipo + refcount = 16 bytes), valor numérico (8+ bytes) e alinhamento. Isso significa que 1 bilhão de somações aloca e desaloca bilhões de objetos na heap, gerando pressão enorme no garbage collector e nas caches L1/L2.

---

## 5. Como Comprovar em Aula

### 5.1 Disassembly (objdump)

O comando `objdump -d` mostra exatamente o que a CPU vai executar:

```bash
$ objdump -d soma_asm | grep -A 5 loop

0000000000401008 <_start.loop>:
  401008:  48 01 c8    add    %rcx,%rax    # 3 bytes
  40100b:  48 ff c9    dec    %rcx          # 3 bytes
  40100e:  75 f8       jne    401008        # 2 bytes

# Total: 8 bytes para o loop inteiro. Zero overhead.
```

### 5.2 Assembly Gerado pelo GCC (`gcc -S`)

Compare o que o compilador gera em `-O0` vs `-O3`:

```bash
$ gcc -O0 -S soma_vetor.c -o soma_O0.s
$ gcc -O3 -S soma_vetor.c -o soma_O3.s
$ diff soma_O0.s soma_O3.s

# -O0: usa memória (stack) a cada iteração
# -O3: mantém variáveis em registradores
```

### 5.3 Bytecode Python (dis)

```bash
$ python3 -m dis soma_vetor.py

# Mostra os OPCODES da VM do CPython:
#   LOAD_FAST, BINARY_ADD, STORE_FAST...
# Estes NÃO são instruções de CPU!
# Cada opcode é interpretado por um switch/case em C
```

### 5.4 Syscalls (strace)

```bash
$ strace -c ./soma_asm    # 3-4 syscalls no total
$ strace -c ./soma_c_O0   # dezenas de syscalls (libc init)

# O binário ASM faz o mínimo: write + exit
# O C carrega a libc antes de começar
```

### 5.5 Tamanho dos Binários

```bash
$ ls -lh soma_asm soma_c_O3
  8.9K soma_asm     # só seu código + dados
   16K soma_c_O3    # inclui startup da libc

$ size soma_asm
  text  data  bss  dec   hex  filename
   142    9    20  171    ab  soma_asm

# 142 bytes de código executável!
```

---

## 6. Lições para o 3º Período

### Lição 1: Abstração tem custo

Cada camada de abstração (interpretador, tipagem dinâmica, garbage collector) adiciona instruções de máquina que a CPU precisa executar. Python não é "lento" — ele faz **muito mais trabalho por operação** porque oferece muito mais conveniência ao programador.

### Lição 2: O compilador é (quase) tão bom quanto você

C com `-O3` atingiu tempo quase idêntico ao Assembly escrito à mão. Compiladores modernos (GCC, Clang) aplicam: eliminação de código morto, promoção de variáveis para registradores, vetorização automática (SIMD), pipeline scheduling. Na prática, **escrever Assembly manualmente raramente compensa** — o compilador faz isso por você.

### Lição 3: Algoritmo > Linguagem

A fórmula de Gauss em Python (0.005 ms) derrotou o Assembly puro (400 ms) por um fator de **80.000x**. Isso demonstra que **um bom algoritmo na linguagem "errada" supera um mau algoritmo na linguagem "certa"**. Antes de otimizar o hardware, otimize a lógica.

### Lição 4: Entender o hardware ajuda em QUALQUER linguagem

Saber que Python aloca objetos na heap para cada inteiro explica por que `sum(range(N))` (implementado em C) é 6x mais rápido que o loop puro. Entender cache, registradores e pipelines ajuda a escrever código melhor em *qualquer* linguagem.

### Lição 5: Cada ferramenta tem seu domínio

| Linguagem | Ideal para | Evitar quando |
|---|---|---|
| **Assembly** | Bootloaders, drivers, crypto, ISR | Qualquer coisa com > 500 linhas |
| **C** | Kernels, embarcados, game engines, DBs | Prototipagem rápida, web apps |
| **Python** | ML/AI, scripts, automação, web, CI/CD | Loops CPU-bound com bilhões de iterações |

---

## 7. Exercícios Propostos

1. **[Observação]** Remova o `volatile` do código C e compile com `-O3`. O que acontece? Use `gcc -S` para descobrir o que o compilador fez.
2. **[Análise]** Execute `strace -c ./soma_asm` e `strace -c ./soma_c_O0`. Compare o número de syscalls. Por que são diferentes?
3. **[Bytecode]** Use `python3 -m dis` para visualizar o bytecode do loop Python. Quantas instruções de VM são executadas por iteração?
4. **[Desafio]** Modifique o código Assembly para usar a fórmula de Gauss (N*(N+1)/2) em vez do loop. Quantas instruções são necessárias?
5. **[Reflexão]** Se Python é ~185x mais lento que Assembly neste benchmark, por que empresas como Google, Instagram e Dropbox usam Python? Escreva 1 parágrafo justificando.

---

---

# GUIA PASSO A PASSO: Como Executar os Benchmarks

> Este guia cobre **Linux (Ubuntu/Debian)**, **macOS** e **Windows**.  
> Todos os comandos assumem que os 4 arquivos (`soma_vetor.asm`, `soma_vetor.c`, `soma_vetor.py`, `benchmark_completo.sh`) estão na mesma pasta.

---

## PASSO 0 — Pré-requisitos (instalar ferramentas)

### Linux (Ubuntu / Debian)

```bash
# Atualizar e instalar tudo de uma vez
sudo apt update
sudo apt install -y nasm gcc python3 binutils

# Verificar instalação
nasm --version    # NASM version 2.16+
gcc --version     # gcc 12+
python3 --version # Python 3.10+
objdump --version # GNU objdump
```

### macOS

```bash
# Instalar Homebrew (se ainda não tem)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instalar ferramentas
xcode-select --install   # instala gcc (na verdade clang) e binutils
brew install nasm python3

# Verificar
nasm --version
gcc --version      # Apple clang (funciona igual para nosso .c)
python3 --version
```

> **NOTA macOS:** O Assembly x86-64 (NASM) só funciona nativamente em Macs Intel. Em Macs com Apple Silicon (M1/M2/M3/M4), use uma VM com Ubuntu x86-64 (UTM ou Docker) ou execute apenas o C e Python.

### Windows

**Opção A — WSL (Recomendado, mais fácil):**

```powershell
# No PowerShell como Administrador:
wsl --install

# Após reiniciar, abra o Ubuntu no WSL e siga o guia do Linux acima:
sudo apt update
sudo apt install -y nasm gcc python3 binutils
```

**Opção B — Ferramentas nativas no Windows (sem WSL):**

```powershell
# 1. NASM: Baixe o instalador em https://www.nasm.us/pub/nasm/releasebuilds/
#    Adicione ao PATH do sistema após instalar

# 2. GCC: Instale MinGW-w64
#    Baixe em https://winlibs.com/ (versão GCC 13+)
#    Extraia e adicione a pasta bin/ ao PATH

# 3. Python: Baixe em https://www.python.org/downloads/
#    Marque "Add Python to PATH" durante a instalação

# 4. Verificar no Prompt de Comando (cmd):
nasm --version
gcc --version
python --version
```

> **NOTA Windows nativo:** O Assembly NASM para Linux (syscalls) **não funciona** no Windows sem WSL. O `.asm` usa `syscall` do Linux. No Windows nativo, você executa apenas o `.c` e o `.py`. Para a experiência completa, use WSL.

---

## PASSO 1 — Organizar os arquivos

### Linux / macOS / WSL

```bash
# Criar pasta de trabalho e entrar nela
mkdir -p ~/benchmark-hardware
cd ~/benchmark-hardware

# Copiar os 4 arquivos para esta pasta
# (ajuste o caminho de onde você baixou os arquivos)
cp ~/Downloads/soma_vetor.asm .
cp ~/Downloads/soma_vetor.c .
cp ~/Downloads/soma_vetor.py .
cp ~/Downloads/benchmark_completo.sh .

# Verificar que estão todos aqui
ls -la
# Deve mostrar: soma_vetor.asm  soma_vetor.c  soma_vetor.py  benchmark_completo.sh
```

### Windows (cmd nativo, sem WSL)

```cmd
:: Criar pasta e entrar
mkdir %USERPROFILE%\benchmark-hardware
cd %USERPROFILE%\benchmark-hardware

:: Copiar os arquivos (ajuste o caminho)
copy %USERPROFILE%\Downloads\soma_vetor.c .
copy %USERPROFILE%\Downloads\soma_vetor.py .

:: Verificar
dir
```

---

## PASSO 2 — Compilar e executar o ASSEMBLY (Linux / WSL apenas)

```bash
cd ~/benchmark-hardware

# ─────────────────────────────────────────
# ETAPA 2.1: Montar (assembler) o .asm
# ─────────────────────────────────────────
# nasm transforma o código Assembly em um arquivo objeto (.o)
# -f elf64 = formato ELF para Linux 64-bit
nasm -f elf64 soma_vetor.asm -o soma_vetor.o

# ─────────────────────────────────────────
# ETAPA 2.2: Linkar (gerar o executável)
# ─────────────────────────────────────────
# ld = linker do GNU, transforma o .o em binário executável
# NÃO usa libc — link direto sem biblioteca
ld soma_vetor.o -o soma_asm

# ─────────────────────────────────────────
# ETAPA 2.3: Verificar o que foi gerado
# ─────────────────────────────────────────
ls -lh soma_asm
# Saída esperada: ~8.9K (menor que um tweet em bytes!)

file soma_asm
# Saída: ELF 64-bit LSB executable, x86-64, statically linked

size soma_asm
# Mostra: text=142  data=9  bss=20
# 142 bytes de código executável!

# ─────────────────────────────────────────
# ETAPA 2.4: Executar
# ─────────────────────────────────────────
./soma_asm
# Saída: Soma = 500000000500000000
# Tempo esperado: ~400ms (1 bilhão de iterações!)

# ─────────────────────────────────────────
# ETAPA 2.5: Medir tempo externamente
# ─────────────────────────────────────────
# Usar date porque 'time' nem sempre está disponível
start=$(date +%s%N); ./soma_asm; end=$(date +%s%N)
echo "Tempo: $(( (end - start) / 1000000 )) ms"

# ─────────────────────────────────────────
# ETAPA 2.6: Inspecionar o código de máquina
# ─────────────────────────────────────────
# Disassembly — ver as instruções reais da CPU
objdump -d soma_asm

# Filtrar só o loop (a parte que importa)
objdump -d soma_asm | grep -A 5 "loop"
# Saída:
#   401008: 48 01 c8    add  %rcx,%rax   ← soma += contador
#   40100b: 48 ff c9    dec  %rcx         ← contador--
#   40100e: 75 f8       jne  401008       ← se != 0, repete
# 3 instruções. 8 bytes. Isso é TUDO que a CPU faz.
```

---

## PASSO 3 — Compilar e executar o C

### Linux / macOS / WSL

```bash
cd ~/benchmark-hardware

# ─────────────────────────────────────────
# ETAPA 3.1: Compilar SEM otimização (-O0)
# ─────────────────────────────────────────
gcc -O0 soma_vetor.c -o soma_c_O0

# ─────────────────────────────────────────
# ETAPA 3.2: Compilar COM otimização (-O2 e -O3)
# ─────────────────────────────────────────
gcc -O2 soma_vetor.c -o soma_c_O2
gcc -O3 soma_vetor.c -o soma_c_O3

# ─────────────────────────────────────────
# ETAPA 3.3: Verificar tamanhos
# ─────────────────────────────────────────
ls -lh soma_c_O0 soma_c_O2 soma_c_O3
# Todos ~16KB (inclui startup da libc)

# ─────────────────────────────────────────
# ETAPA 3.4: Executar os 3 e comparar
# ─────────────────────────────────────────
echo "=== SEM otimização (-O0) ==="
./soma_c_O0
# Esperado: ~850 ms

echo ""
echo "=== Otimização média (-O2) ==="
./soma_c_O2
# Esperado: ~420 ms

echo ""
echo "=== Otimização máxima (-O3) ==="
./soma_c_O3
# Esperado: ~420 ms (quase igual -O2 neste caso)

# ─────────────────────────────────────────
# ETAPA 3.5: Ver o assembly que o GCC gerou
# ─────────────────────────────────────────
# -S gera arquivo .s (assembly legível) em vez de binário
gcc -O0 -S soma_vetor.c -o soma_O0.s
gcc -O3 -S soma_vetor.c -o soma_O3.s

# Comparar lado a lado
diff soma_O0.s soma_O3.s

# Ver o loop no -O0 (usa memória/stack):
grep -A 8 ".L3:" soma_O0.s
# movq -72(%rbp),%rdx   ← lê da stack
# addq %rdx,%rax        ← soma
# movq %rax,-72(%rbp)   ← escreve na stack (!) volta e meia

# Ver o loop no -O3 (usa registradores):
grep -A 6 ".L2:" soma_O3.s
# addq %rax,%rdx        ← tudo em registradores, sem tocar na stack

# ─────────────────────────────────────────
# ETAPA 3.6: EXPERIMENTO — Remover volatile
# ─────────────────────────────────────────
# Edite soma_vetor.c: troque "volatile int64_t soma = 0"
# por apenas "int64_t soma = 0"
# Depois:
#   gcc -O3 soma_vetor.c -o soma_c_sem_volatile
#   ./soma_c_sem_volatile
# SURPRESA: o tempo cai para ~0 ms!
# O compilador calculou N*(N+1)/2 em tempo de compilação!
# Verifique: gcc -O3 -S soma_vetor.c -o sem_volatile.s
# O loop DESAPARECEU do assembly!
```

### Windows (cmd nativo com MinGW)

```cmd
cd %USERPROFILE%\benchmark-hardware

:: Compilar (MinGW usa o mesmo gcc)
gcc -O0 soma_vetor.c -o soma_c_O0.exe
gcc -O3 soma_vetor.c -o soma_c_O3.exe

:: Executar
soma_c_O0.exe
soma_c_O3.exe

:: Ver assembly gerado
gcc -O0 -S soma_vetor.c -o soma_O0.s
gcc -O3 -S soma_vetor.c -o soma_O3.s
notepad soma_O0.s
notepad soma_O3.s
```

---

## PASSO 4 — Executar o Python

### Linux / macOS / WSL

```bash
cd ~/benchmark-hardware

# ─────────────────────────────────────────
# ETAPA 4.1: Executar o benchmark completo
# ─────────────────────────────────────────
python3 soma_vetor.py
# Roda 3 testes:
#   [1] Loop puro (100M)     → ~7.450 ms  (⚠ LENTO!)
#   [2] sum(range()) (1bi)   → ~13.500 ms
#   [3] Gauss O(1)           → ~0.005 ms  (instantâneo)
# + Mostra o bytecode do loop

# ATENÇÃO: o teste [1] demora ~7 segundos. Paciência!

# ─────────────────────────────────────────
# ETAPA 4.2: Ver o bytecode (instruções da VM Python)
# ─────────────────────────────────────────
python3 -m dis soma_vetor.py
# Mostra TODOS os opcodes que o interpretador vai executar
# Compare: cada opcode Python ≠ instrução de CPU
# Cada BINARY_ADD é na verdade ~50 instruções x86 por baixo!

# ─────────────────────────────────────────
# ETAPA 4.3: Medir só o loop puro com time
# ─────────────────────────────────────────
python3 -c "
import time
N = 100_000_000
t = time.perf_counter()
s = 0
for i in range(1, N+1): s += i
print(f'Soma={s}  Tempo={(time.perf_counter()-t)*1000:.0f} ms')
"

# ─────────────────────────────────────────
# ETAPA 4.4: Medir o tamanho de um int Python
# ─────────────────────────────────────────
python3 -c "
import sys
print(f'int 0:    {sys.getsizeof(0)} bytes')
print(f'int 1:    {sys.getsizeof(1)} bytes')
print(f'int 2**30: {sys.getsizeof(2**30)} bytes')
print(f'int 2**60: {sys.getsizeof(2**60)} bytes')
print(f'int 2**1000: {sys.getsizeof(2**1000)} bytes')
print()
print('Em C/Assembly, TODOS seriam 8 bytes (int64_t).')
"
# Saída esperada:
#   int 0:     28 bytes  (!)
#   int 1:     28 bytes
#   int 2**30: 28 bytes
#   int 2**60: 32 bytes  (cresceu!)
#   int 2**1000: 164 bytes (inteiro de precisão arbitrária)
```

### Windows (cmd nativo)

```cmd
cd %USERPROFILE%\benchmark-hardware

:: Executar (use "python" em vez de "python3" no Windows)
python soma_vetor.py

:: Ver bytecode
python -m dis soma_vetor.py

:: Medir tamanho dos inteiros
python -c "import sys; print(sys.getsizeof(0), 'bytes para o inteiro 0')"
```

---

## PASSO 5 — Executar TUDO de uma vez (Linux / WSL)

```bash
cd ~/benchmark-hardware

# Dar permissão de execução ao script master
chmod +x benchmark_completo.sh

# Rodar!
./benchmark_completo.sh

# Este script:
#   1. Verifica dependências
#   2. Compila ASM e C (O0, O2, O3)
#   3. Gera assembly legível (.s)
#   4. Executa todos os benchmarks
#   5. Mostra tabela comparativa + comandos de exploração
```

---

## PASSO 6 — Ferramentas de análise avançada

### 6.1 strace — ver syscalls (Linux / WSL)

```bash
# Syscalls do Assembly (mínimas — só write e exit)
strace -c ./soma_asm
# Saída: 3-4 syscalls no total

# Syscalls do C (inclui inicialização da libc)
strace -c ./soma_c_O0
# Saída: dezenas de syscalls (mmap, brk, access...)

# Ver as syscalls em tempo real (modo verboso)
strace ./soma_asm 2>&1 | head -20
```

### 6.2 objdump — disassembly (Linux / macOS / WSL)

```bash
# Ver TODO o código de máquina do binário ASM
objdump -d soma_asm

# Ver só o loop (a parte que importa)
objdump -d soma_asm | grep -A 5 "loop"

# Comparar com o binário C
objdump -d soma_c_O3 | grep -A 15 ".L2"

# macOS: use otool em vez de objdump
# otool -tv soma_c_O3
```

### 6.3 size — comparar seções dos binários (Linux / macOS / WSL)

```bash
# Mostra text (código), data (dados), bss (não-inicializados)
size soma_asm soma_c_O0 soma_c_O3

# Saída esperada:
#    text  data  bss   dec   hex  filename
#     142    9    20   171    ab  soma_asm     ← 142 bytes de código!
#    1834  616     8  2458   99a  soma_c_O0
#    1794  616     8  2418   972  soma_c_O3
```

### 6.4 perf — profiling de CPU (Linux, requer permissões)

```bash
# Se disponível (pode precisar de sudo):
sudo perf stat ./soma_asm
sudo perf stat ./soma_c_O3
# Mostra: ciclos, instruções, IPC (instructions per cycle),
# cache misses, branch mispredictions
```

---

## PASSO 7 — Tabela resumo por sistema operacional

| Recurso | Linux (Ubuntu) | macOS | Windows (cmd) | Windows (WSL) |
|---|---|---|---|---|
| **Assembly (NASM)** | ✅ Completo | ⚠️ Só Intel | ❌ Syscalls Linux | ✅ Completo |
| **C (GCC)** | ✅ Completo | ✅ Clang (compatível) | ✅ MinGW | ✅ Completo |
| **Python** | ✅ Completo | ✅ Completo | ✅ Completo | ✅ Completo |
| **objdump** | ✅ | ✅ (ou otool) | ❌ | ✅ |
| **strace** | ✅ | ❌ (use dtrace) | ❌ | ✅ |
| **gcc -S** | ✅ | ✅ | ✅ MinGW | ✅ |
| **python3 -m dis** | ✅ | ✅ | ✅ | ✅ |
| **benchmark_completo.sh** | ✅ | ⚠️ Sem ASM em ARM | ❌ | ✅ |

**Recomendação para os alunos:** Se usam Windows, instalem o WSL (leva 5 minutos) para ter a experiência completa. Se usam Mac com Apple Silicon, usem Docker com imagem Ubuntu x86-64 para o Assembly.

---

**Arquivos do benchmark:** `soma_vetor.asm` | `soma_vetor.c` | `soma_vetor.py` | `benchmark_completo.sh`
