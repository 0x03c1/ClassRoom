# Aula 1 — Anatomia da CPU: Pipeline e Multicore

## Descrição

Este bloco abre a aula com a **pergunta-âncora**: *"Por que dois computadores aparentemente similares têm desempenhos tão diferentes?"*. Em seguida, os alunos inspecionam a CPU usando ferramentas em Windows e Linux, e observam empiricamente o ganho de paralelismo executando o **Cinebench R23** em modos single e multi-core. O bloco termina com a primeira evidência: **a frequência sozinha não explica o desempenho**.

## Objetivos de Aprendizagem

Ao final deste bloco, o aluno deve ser capaz de:
- Identificar núcleos físicos vs threads lógicas em uma CPU
- Distinguir frequência base, turbo e conjuntos de instruções (SSE, AVX)
- Calcular fator de escala paralelo e relacioná-lo com a Lei de Amdahl
- Explicar, em linhas gerais, o que "quebra" o pipeline (hazards, branch misprediction)

## Antes de começar (10 min — Abertura da aula inteira)

1. Apresente no projetor **dois cenários de máquinas** (use prints de CPU-Z de máquinas reais ou monte um mockup): mesma frequência nominal, mesma RAM, mas tempos de compilação muito diferentes para o mesmo projeto.
2. Pergunte à turma: *"Onde mora a diferença?"*
3. Liste no quadro **todas as hipóteses** levantadas. Ao final do laboratório (Bloco 5), retomamos esta lista e marcamos cada hipótese como confirmada / refutada / parcial.
4. Cada dupla registra no relatório: *"Quais 5 métricas você usaria, agora, para comparar duas máquinas?"*

> **Dica pedagógica:** não corrija nem valide hipóteses neste momento. A força do método investigativo está em deixar os alunos *descobrirem* ao longo da aula.

---

## Passo a Passo da Prática

### Atividade 1.1 — Inspeção da CPU (10 min)

#### No Windows

1. **Abra o CPU-Z.** Na aba "CPU", anote:
   - Nome do processador e codename
   - Specification (frequência nominal)
   - Núcleos (Cores) e Threads
   - Tamanho de cache L1, L2, L3 (já conectando ao próximo bloco)
   - Instruction set (procurar SSE, AVX, AVX2, AVX-512)

2. **Abra o HWiNFO64** em modo "Sensors only". Observe a frequência **em tempo real** de cada core. Note como ela varia com a carga (Turbo Boost, SpeedStep).

3. Tirem um screenshot e colem no relatório.

#### No Linux

```bash
# Visão geral da CPU
lscpu

# Topologia hierárquica visual (gera PNG)
lstopo --of png > /tmp/topologia-cpu.png
xdg-open /tmp/topologia-cpu.png

# Frequência em tempo real de cada core
watch -n 1 "grep 'cpu MHz' /proc/cpuinfo"
```

> Os alunos devem identificar no `lscpu` os campos: `Socket(s)`, `Core(s) per socket`, `Thread(s) per core`, `CPU max MHz`, `Flags` (procurar `sse4_2`, `avx`, `avx2`).

### Atividade 1.2 — Pipeline e Paralelismo na Prática (15 min)

#### Experimento 1: Cinebench R23 (Windows) — 8 min

1. Abra o **Cinebench R23**.
2. Execute primeiro **Single Core** (clique no botão "Start" da linha Single Core). Aguarde ~10 min — *como o tempo é apertado, configure o teste mínimo em "File → Advanced benchmark → Custom Minimum Test Duration: Off"*.
3. Anote a pontuação **single-core**.
4. Execute em seguida **Multi Core** com a mesma configuração. Anote a pontuação.
5. Calcule:

   ```
   Fator de escala = Pontuação Multi / Pontuação Single
   ```

6. Compare com o **número de threads lógicas** identificado na atividade 1.1.

#### Experimento 2: Sysbench (Linux) — alternativa ou complementar — 7 min

```bash
# Single thread
sysbench cpu --threads=1 --time=10 run | grep "events per second"

# Multi-thread (substitua N pelo número de threads da sua CPU)
sysbench cpu --threads=N --time=10 run | grep "events per second"
```

#### Experimento 3: Script Python para visualizar o paralelismo — 5 min

Use o script [`scripts/teste_paralelismo.py`](./scripts/teste_paralelismo.py) deste repositório (funciona igual no Windows e Linux):

```bash
python3 scripts/teste_paralelismo.py
```

Este script executa a mesma carga computacional (multiplicação de matrizes) em **1, 2, 4, 8 threads** e gera uma tabela com tempos e fator de escala — útil para discutir Lei de Amdahl em concreto.

### Tabela de Resultados (preencher no relatório)

| Métrica | Valor |
|---------|-------|
| Modelo da CPU | |
| Núcleos físicos / Threads lógicas | / |
| Frequência base (GHz) | |
| Frequência turbo observada (GHz) | |
| Cache L1 / L2 / L3 | / / |
| Instruction sets relevantes | |
| Cinebench Single Core | |
| Cinebench Multi Core | |
| **Fator de escala (Multi/Single)** | |
| **Fator ideal (= nº de threads)** | |
| **Eficiência paralela (%)** | |

---

## Perguntas Reflexivas

Os alunos respondem **no relatório** ao final do bloco:

1. Por que o fator de escala observado é **menor** que o número de threads? Cite ao menos dois motivos técnicos.
2. Durante o teste Single Core, abra o Gerenciador de Tarefas (Windows) ou `htop` (Linux). Por que a carga "pula" entre núcleos em vez de fixar em um? Que componente do SO causa isso?
3. Se o pipeline tem **N estágios** ideais, qual é o ganho teórico máximo em throughput? Quais eventos quebram esse ideal? *(Esperado: hazards de dados, branch misprediction, cache misses.)*
4. CPU-Z mostra a frequência em GHz. O Cinebench mostra "pts". Como você traduziria uma na outra usando o conceito de **CPI (Cycles Per Instruction)**?

> **Provocação para discussão em voz alta** (não precisa estar no relatório): *"Se a Intel e a AMD fabricam CPUs com mesma frequência, por que os benchmarks divergem? Onde mora essa diferença, se não no clock?"*

---

## Conexão com o próximo bloco

> *"Vocês mediram que o paralelismo não escala 1:1. Uma das razões é a **disputa por cache compartilhado**. No próximo bloco, vamos medir os tempos de acesso a cada nível da hierarquia de memória — e vão ver que a diferença entre L1 e RAM é de **duas ordens de grandeza**."*

## Scripts deste bloco

- [`scripts/teste_paralelismo.py`](./scripts/teste_paralelismo.py) — benchmark de escala paralela em Python
- [`scripts/info_cpu.sh`](./scripts/info_cpu.sh) — coleta tudo do `lscpu` formatado para o relatório

---

[← Índice](../../README.md) | **Aula 1** | [Aula 2 →](../aula2-hierarquia-memoria-caches/README.md)
