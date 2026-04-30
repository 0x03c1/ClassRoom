# Aula 5 — Síntese: Benchmark Integrado e Resolução do Mistério

Foco em workload, profiling e análise integrada.

## Navegação

- [Voltar ao índice do laboratório](../../README.md)
- [Aula anterior: Barramentos, I/O e Interrupções](../aula4-barramentos-io-interrupcoes/README.md)
- [Modelo de relatório](../../relatorio-template/relatorio.md)

## Ponte com a teoria

Este bloco integra todos os conceitos da disciplina em um único workload
realista. Ao final do profiling, você terá evidências para explicar se o
gargalo dominante está em CPU, cache, memória, armazenamento ou barramento.

## Descrição

Aqui você fecha o ciclo da disciplina. A dupla executa um **workload
realista**, coleta métricas dos subsistemas estudados (CPU, cache, memória e
I/O) e revisita as hipóteses iniciais com base em dados medidos.

## Objetivos de Aprendizagem

Ao final deste bloco, o aluno deve ser capaz de:

- Usar `perf stat` (Linux) ou Windows Performance Recorder para perfilar um workload real
- Identificar qual subsistema é o **gargalo dominante** da sua máquina
- Defender, com dados, decisões de upgrade ou otimização
- Articular como pipeline, cache, memória virtual, PCIe e multicore se inter-relacionam

---

## Passo a Passo da Prática

### Atividade 5.1 — Escolha o seu workload

Cada dupla escolhe **um** workload entre as opções:

| Opção | Workload                                                                         | O que estressa        |
| ----- | -------------------------------------------------------------------------------- | --------------------- |
| A     | Compilar um projeto C médio (clone do `linux/tools/perf` ou similar)             | CPU + I/O + memória   |
| B     | `7z b` (benchmark interno do 7-Zip)                                              | CPU + cache           |
| C     | Multiplicação de matrizes em Python/NumPy 5000x5000                              | CPU + memória + cache |
| D     | `ffmpeg` convertendo um vídeo de 1 min para outro codec                          | CPU intensivo + I/O   |
| E     | Workload do script `scripts/workload_completo.py` (recomendado se houver dúvida) | Tudo, controlado      |

> Recomendação: se houver dúvida na escolha, use o **script `workload_completo.py`**, pois ele exercita CPU, cache e memória de forma controlada e gera resultados comparáveis entre máquinas.

### Atividade 5.2 — Perfilar o workload

#### No Linux — perf stat (poderoso)

```bash
# Substitua "./meu_workload" pelo comando da sua escolha
sudo perf stat -e cycles,instructions,cache-references,cache-misses,\
branch-misses,page-faults,context-switches \
  ./meu_workload
```

**O que olhar na saída:**

| Métrica                           | O que significa                       | Sinal de alarme                  |
| --------------------------------- | ------------------------------------- | -------------------------------- |
| `instructions per cycle (IPC)`    | Quantas instruções por ciclo de clock | < 1.0 → CPU está esperando muito |
| `cache-misses / cache-references` | Taxa de miss                          | > 5% → algoritmo cache-hostil    |
| `branch-misses`                   | Predições erradas                     | Alto → muitos `if` imprevisíveis |
| `page-faults`                     | Faltas de página                      | Alto → pressão de memória        |
| `context-switches`                | Trocas de contexto                    | Alto → muita disputa por CPU     |

#### No Windows — Process Explorer + Performance Monitor

1. Abra o **Process Explorer**, identifique o PID do seu workload.
2. Clique com botão direito → **Properties → Performance**. Observe:
   - CPU Time
   - I/O Reads / Writes
   - Working Set Peak
   - Page Faults

3. Para profiling mais profundo, use o **Windows Performance Recorder** (parte do ADK):
   - Inicie a gravação com perfil "CPU usage"
   - Execute o workload
   - Pare e analise no **Windows Performance Analyzer**

#### Cross-platform — Script Python integrado

```bash
python3 scripts/workload_completo.py
```

Este script:

- Executa fases controladas: **CPU-bound**, **memory-bound**, **I/O-bound**
- Coleta métricas de CPU, memória e disco em paralelo (via psutil)
- Gera um relatório final com inferência de gargalo

### Atividade 5.3 — Tabela de evidências

Preencha no relatório:

| Subsistema      | Métrica medida                | Valor da minha máquina |
| --------------- | ----------------------------- | ---------------------- |
| Pipeline (CPU)  | IPC (instructions per cycle)  |                        |
| Cache           | Taxa de cache miss (%)        |                        |
| Memória virtual | Page faults / s               |                        |
| Armazenamento   | Throughput durante o workload |                        |
| Multicore       | Threads efetivamente em uso   |                        |
| PCIe            | Saturação do link PCIe do SSD |                        |

### Atividade 5.4 — Resolução do mistério

Retome **a lista de hipóteses iniciais** (do Bloco 0). Classifique cada hipótese:

- **Confirmada** — há evidência medida que sustenta
- **Refutada** — os dados contradizem
- **Parcial** — depende do cenário; explique qual

> Este passo fecha a investigação: a resposta final deve ser defendida por métricas, não por impressão.

---

## Perguntas Reflexivas Finais (entrega no relatório)

Estas são as perguntas-síntese, valem como nota:

1. Sua máquina tem mais "gargalo" de **CPU**, **memória**, **armazenamento** ou **barramento**? Justifique citando **três métricas concretas medidas hoje**.

2. Se você tivesse **R$ 500** para investir em upgrade da máquina, onde investiria? **Defenda a escolha tecnicamente**, com base no gargalo identificado.

3. Em que cenário um **Xeon de 32 núcleos a 2.5 GHz** ganha de um **Core i9 de 8 núcleos a 5.5 GHz**? Em que cenário perde? Use a Lei de Amdahl em sua resposta.

4. Em **uma frase cada**, explique como cada conceito abaixo afetou o tempo de execução do seu workload escolhido:
   - Pipeline e instruction-level parallelism
   - Hierarquia de cache
   - Memória virtual
   - PCIe / barramento
   - Multicore

---

## Encerramento

### Síntese visual

Represente o **caminho de uma instrução**, da memória virtual até a unidade de execução, marcando onde cada tópico da aula entra:

```text
Programa em C
     ↓
[Memória Virtual] ── page fault? ──→ [SSD via PCIe]
     ↓                                       ↑
[RAM] ←─────────────────── DMA + IRQ ────────┘
     ↓
[Cache L3] ← compartilhado entre cores
     ↓
[Cache L2/L1]
     ↓
[Pipeline da CPU] → multicore (vários pipelines em paralelo)
     ↓
RESULTADO
```

### Tarefa de casa

1. **Repetir o workload escolhido em uma máquina diferente** (a do laboratório vs casa, ou a de outro colega).
2. Comparar as métricas. Onde estão as maiores diferenças? Por quê?
3. Entregar o relatório completo em **1 semana**.

### Próximo encontro

Conecte seus resultados com o próximo tópico do plano de aula (pipeline detalhado com hazards, **cache coherence em multicore** ou arquiteturas SIMD/GPU).

---

## Scripts deste bloco

- [`scripts/workload_completo.py`](./scripts/workload_completo.py) — workload integrado para profiling
