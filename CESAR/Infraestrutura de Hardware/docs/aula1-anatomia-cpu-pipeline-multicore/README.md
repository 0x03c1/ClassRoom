# Aula 1 — Anatomia da CPU: Pipeline e Multicore

Foco em pipeline, multicore, paralelismo e Lei de Amdahl.

## Navegação

- [Voltar ao índice do laboratório](../../README.md)
- [Próxima aula: Hierarquia de Memória e Caches](../aula2-hierarquia-memoria-caches/README.md)

## Ponte com a teoria

Este bloco conecta pipeline, ISA e paralelismo com medições reais. Ao comparar resultados single-core e multi-core, você observa na prática que clock nominal não explica desempenho sozinho.

## Descrição

Neste bloco, você investiga a pergunta-âncora:
_"Por que dois computadores aparentemente similares têm desempenhos tão
 diferentes?"_. A prática começa com inspeção da CPU (Windows e Linux),
segue com medições de paralelismo (single-core e multi-core) e fecha com a
primeira conclusão: **frequência sozinha não explica desempenho**.

## Objetivos de Aprendizagem

Ao final deste bloco, o aluno deve ser capaz de:

- Identificar núcleos físicos vs threads lógicas em uma CPU
- Distinguir frequência base, turbo e conjuntos de instruções (SSE, AVX)
- Calcular fator de escala paralelo e relacioná-lo com a Lei de Amdahl
- Explicar, em linhas gerais, o que "quebra" o pipeline (hazards, branch misprediction)

## Contexto inicial

1. Observe os dois cenários de máquinas apresentados na abertura: mesma frequência nominal, mesma RAM, mas tempos de compilação diferentes.
2. Registre suas hipóteses para explicar a diferença.
3. Anote no relatório as 5 métricas que você usaria para comparar máquinas neste momento.
4. Guarde essas hipóteses para revisar no Bloco 5 (confirmada, refutada ou parcial).

---

## Passo a Passo da Prática

### Atividade 1.1 — Inspeção da CPU

#### No Windows

1. **Abra o CPU-Z.** Na aba "CPU", anote:
   - Nome do processador e codename
   - Specification (frequência nominal)
   - Núcleos (Cores) e Threads
   - Tamanho de cache L1, L2, L3 (já conectando ao próximo bloco)
   - Instruction set (procurar SSE, AVX, AVX2, AVX-512)

2. **Abra o HWiNFO64** em modo "Sensors only". Observe a frequência **em tempo real** de cada core. Note como ela varia com a carga (Turbo Boost, SpeedStep).

3. Tire um screenshot e cole no relatório.

#### No Linux

> **Importante:** os comandos `lscpu` e `/proc/cpuinfo` são específicos de
> Linux. Alunos com macOS, vejam a seção macOS abaixo. Alunos com Windows
> usam CPU-Z e HWiNFO64 (já cobertos).

```bash
# Visão geral da CPU (todas as arquiteturas Linux)
lscpu

# Topologia hierárquica visual (gera PNG)
lstopo --of png > /tmp/topologia-cpu.png
xdg-open /tmp/topologia-cpu.png

# Frequência em tempo real — Linux x86 clássico
watch -n 1 "grep 'cpu MHz' /proc/cpuinfo"

# Em ARM Linux (Raspberry Pi, Graviton, máquinas Snapdragon)
# ou se /proc/cpuinfo não tiver 'cpu MHz', use:
watch -n 1 "cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq 2>/dev/null"
```

> No `lscpu`, identifique os campos: `Socket(s)`, `Core(s) per socket`,
> `Thread(s) per core`, `CPU max MHz`, `Flags` (procure `sse4_2`, `avx`,
> `avx2`). Em ARM, o campo equivalente a `Flags` é `Features` e os nomes
> mudam (`neon`, `asimd`, `sve`, etc.).

#### No macOS (Apple Silicon ou Intel)

macOS não tem `/proc/cpuinfo`. Use:

```bash
# Modelo, núcleos e cache
sysctl -n machdep.cpu.brand_string
sysctl -n hw.ncpu hw.physicalcpu hw.logicalcpu
sysctl -a | grep cachesize

# Frequência em tempo real: abra o Activity Monitor e use
# Window → CPU History, ou instale `mactop` via Homebrew.
```

### Atividade 1.2 — Pipeline e Paralelismo na Prática

#### Experimento 1: Cinebench R23 (Windows)

> **Importante sobre versões:** baixe especificamente o **Cinebench R23** em
> <https://www.maxon.net/en/downloads/cinebench-downloads> (a página oficial
> ainda lista R23 junto com 2024 e 2026). **Não use 2024 ou 2026** — a
> Maxon recalibrou a escala de pontos, então scores não são comparáveis
> entre versões. R23 é o que tem a maior base pública de comparação.

1. Abra o **Cinebench R23**.
2. No menu superior: **File → Advanced benchmark**. Marque essa opção.
3. Em **Minimum Test Duration**, escolha **Off** (caso contrário, o teste
   roda por 10 min — fora da janela da aula).
4. Execute primeiro **CPU (Single Core)** clicando em "Start". Anote a
   pontuação.
5. Execute em seguida **CPU (Multi Core)**. Anote a pontuação.
6. Calcule:

   ```text
   Fator de escala = Pontuação Multi / Pontuação Single
   ```

7. Compare com o **número de threads lógicas** identificado na atividade 1.1.

#### Experimento 2: Sysbench (Linux) — alternativa ou complementar

```bash
# Single thread
sysbench cpu --threads=1 --time=10 run | grep "events per second"

# Multi-thread (substitua N pelo número de threads da sua CPU)
sysbench cpu --threads=N --time=10 run | grep "events per second"
```

#### Experimento 3: Script Python para visualizar o paralelismo

Use o script [`scripts/teste_paralelismo.py`](./scripts/teste_paralelismo.py) deste repositório (funciona igual no Windows e Linux):

```bash
python3 scripts/teste_paralelismo.py
```

Este script executa a mesma carga computacional (multiplicação de matrizes) em **1, 2, 4, 8 threads** e gera uma tabela com tempos e fator de escala — útil para discutir Lei de Amdahl em concreto.

### Tabela de Resultados (preencher no relatório)

| Métrica                            | Valor |
| ---------------------------------- | ----- |
| Modelo da CPU                      |       |
| Núcleos físicos / Threads lógicas  | /     |
| Frequência base (GHz)              |       |
| Frequência turbo observada (GHz)   |       |
| Cache L1 / L2 / L3                 | / /   |
| Instruction sets relevantes        |       |
| Cinebench Single Core              |       |
| Cinebench Multi Core               |       |
| **Fator de escala (Multi/Single)** |       |
| **Fator ideal (= nº de threads)**  |       |
| **Eficiência paralela (%)**        |       |

---

## Perguntas Reflexivas

Registre as respostas **no relatório** ao final do bloco:

1. Por que o fator de escala observado é **menor** que o número de threads? Cite ao menos dois motivos técnicos.
2. Durante o teste Single Core, abra o Gerenciador de Tarefas (Windows) ou `htop` (Linux). Por que a carga "pula" entre núcleos em vez de fixar em um? Que componente do SO causa isso?
3. Se o pipeline tem **N estágios** ideais, qual é o ganho teórico máximo em throughput? Quais eventos quebram esse ideal? _(Esperado: hazards de dados, branch misprediction, cache misses.)_
4. CPU-Z mostra a frequência em GHz. O Cinebench mostra "pts". Como você traduziria uma na outra usando o conceito de **CPI (Cycles Per Instruction)**?

> Discussão opcional individual: _"Se Intel e AMD fabricam CPUs com mesma frequência, por que os benchmarks divergem? Onde está essa diferença, além do clock?"_

---

## Atividade opcional — Conectando com Assembly e MARS

> Esta atividade é **opcional** e conecta o experimento de benchmark com o que você já praticou em MARS.

1. Abra o **MARS** (ou qualquer simulador MIPS) e carregue um programa simples — pode ser uma soma em loop:

   ```mips
   .data
   resultado: .word 0
   .text
   main:
       li   $t0, 0          # acumulador
       li   $t1, 1000000    # contador
   loop:
       add  $t0, $t0, $t1   # operação aritmética
       addi $t1, $t1, -1    # decrementa contador
       bnez $t1, loop       # branch condicional
       sw   $t0, resultado
       li   $v0, 10
       syscall
   ```

2. **Identifique visualmente no código** quais instruções:
   - Causam **dependência de dados** entre instruções consecutivas (`add` depende de `addi`?)
   - Geram **branch** que o predictor pode errar (`bnez`)

3. **Reflita:** essas são situações que quebram o pipeline e ajudam a explicar perdas de desempenho vistas no Cinebench.

> Este exercício conecta o que você escreveu no MARS com o que acontece em escala real dentro da CPU durante o benchmark.

---

## Conexão com o próximo bloco

> Você mediu que o paralelismo não escala 1:1. No próximo bloco, você vai medir os tempos de acesso em cada nível da hierarquia de memória e relacionar isso à escalabilidade.

## Scripts deste bloco

- [`scripts/teste_paralelismo.py`](./scripts/teste_paralelismo.py) — benchmark de escala paralela em Python
- [`scripts/info_cpu.sh`](./scripts/info_cpu.sh) — coleta tudo do `lscpu` formatado para o relatório
