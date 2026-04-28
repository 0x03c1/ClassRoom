# Ponte Teoria ↔ Prática

> **Para quê este documento?**
> A disciplina cobriu 14 tópicos teóricos ao longo do semestre.
> O laboratório prático cobre, em 2h, um subconjunto deles,
> mas com **medições reais**, em **máquinas reais**.
> Este guia mapeia, ponto a ponto, **onde cada tópico da ementa aparece no
> laboratório** e oferece sugestões de experimentos opcionais para os tópicos
> mais "abstratos" que não cabem num bloco de tempo curto.
> Use este documento como:
>
> - **Índice reverso:** "Onde, no laboratório, eu vou ver isso que aprendi sobre flip-flops?"
> - **Roteiro de revisão:** antes da prova ou da apresentação final, releia este mapa.
> - **Trampolim para o projeto final:** os tópicos não cobertos aqui podem virar tema de investigação.

## Navegação

- [Voltar ao índice do laboratório](../../README.md)
- [Referência de leitura do htop/top](../referencia-htop-top/README.md)
- [Modelo de relatório](../../relatorio-template/relatorio.md)

---

## Mapa Completo: 14 Tópicos × 5 Blocos do Laboratório

A tabela abaixo é o coração deste guia. Cada tópico da ementa tem ao menos uma referência no material prático.

1. **Multiplexador**
Bloco: [Aula 2](../aula2-hierarquia-memoria-caches/README.md)
Como aparece: MUX seleciona qual *way* do cache associativo entrega o dado no caso de hit.

2. **Demultiplexador**
Bloco: [Aula 2](../aula2-hierarquia-memoria-caches/README.md)
Como aparece: DEMUX faz o decode do endereço para selecionar a célula de memória correta.

3. **Flip-Flop**
Bloco: [Aula 2](../aula2-hierarquia-memoria-caches/README.md)
Como aparece: cada bit de cache SRAM é um flip-flop.

4. **ISA / MIPS**
Bloco: [Aula 1](../aula1-anatomia-cpu-pipeline-multicore/README.md)
Como aparece: CPU-Z mostra extensões da ISA x86 (SSE, AVX).

5. **Assembly**
Bloco: [Aula 1](../aula1-anatomia-cpu-pipeline-multicore/README.md) e [Aula 5](../aula5-sintese-benchmark-integrado/README.md)
Como aparece: Cinebench é Assembly otimizado; `perf annotate` mostra hot spots.

6. **Monociclo / Multiciclo**
Bloco: [Aula 1](../aula1-anatomia-cpu-pipeline-multicore/README.md)
Como aparece: discussão conceitual como base para pipeline.

7. **Pipeline**
Bloco: [Aula 1](../aula1-anatomia-cpu-pipeline-multicore/README.md) e [Aula 5](../aula5-sintese-benchmark-integrado/README.md)
Como aparece: Cinebench e IPC em `perf stat`.

8. **Exceções, Interrupções, I/O**
Bloco: [Aula 4](../aula4-barramentos-io-interrupcoes/README.md)
Como aparece: `/proc/interrupts` e LatencyMon.

9. **Hierarquia de Memória e Cache**
Bloco: [Aula 2](../aula2-hierarquia-memoria-caches/README.md)
Como aparece: AIDA64, sysbench e experimento de localidade.

10. **Memória Principal (RAM)**
Bloco: [Aula 2](../aula2-hierarquia-memoria-caches/README.md) e [Aula 3](../aula3-ram-armazenamento-memoria-virtual/README.md)
Como aparece: largura de banda e comparação com armazenamento.

11. **Armazenamento**
Bloco: [Aula 3](../aula3-ram-armazenamento-memoria-virtual/README.md)
Como aparece: CrystalDiskMark e `fio`.

12. **Memória Virtual**
Bloco: [Aula 3](../aula3-ram-armazenamento-memoria-virtual/README.md)
Como aparece: RAMMap, `vmstat` e `stress-ng`.

13. **Barramento**
Bloco: [Aula 4](../aula4-barramentos-io-interrupcoes/README.md)
Como aparece: `lspci -vv` e comparação com banda de RAM.

14. **Arquitetura Avançada**
Bloco: [Aula 1](../aula1-anatomia-cpu-pipeline-multicore/README.md) e [Aula 5](../aula5-sintese-benchmark-integrado/README.md)
Como aparece: multicore, SIMD, NUMA e MESI.

---

## Tópicos que Merecem um Olhar Extra

Quatro tópicos da ementa são **conceituais demais** ou **lógicos demais**
para serem "medidos" em um laboratório de 2h focado em performance.
Se vocês quiserem aprofundar, aqui estão sugestões de mini-experimentos
opcionais fora do tempo do lab, excelentes para bônus no relatório
ou como tema do projeto final.

### Tópicos 1, 2, 3 — Multiplexador, Demultiplexador, Flip-Flop

Esses três são **lógica digital pura**. A melhor forma de "ver" cada um é construindo no Logisim Evolution. Sugestões:

- **Mini-experimento A — MUX 4:1:** construa um multiplexador 4-para-1 e verifique sua tabela verdade. Em seguida, pesquise como ele aparece dentro de um cache associativo de 4 vias.
- **Mini-experimento B — Flip-Flop D:** construa um flip-flop tipo D no Logisim. Observe como ele "lembra" o valor entre ciclos de clock. Esse é o tijolo do registrador da CPU.
- **Mini-experimento C — Banco de registradores:** combine 32 flip-flops com
        MUX e DEMUX para implementar o banco de registradores do MIPS.
        Vocês vão recriar, na prática, o componente que o MARS simula em alto nível.

> **Para ir além:** a Proposta 3 do Projeto Final (TinyArch) leva esses três
> tópicos ao extremo: vocês constroem uma ALU completa no Logisim,
> integram ao TinyCPU-4 e medem o efeito do cache que vocês implementaram.

### Tópico 4 — ISA / MIPS

ISA é o **contrato** entre hardware e software. No laboratório, esse contrato aparece de forma indireta (via CPU-Z mostrando extensões). Sugestões para aprofundar:

- **Mini-experimento D — Comparação de ISAs:** abra o MARS, escreva um
        programa MIPS que some 100 números. Em seguida, escreva o mesmo programa em C
        e compile com `gcc -S -O0 programa.c -o programa.s` em uma máquina x86.
        Compare o Assembly gerado. Quantas instruções são necessárias em cada ISA?
- **Mini-experimento E — Vendo o Assembly real:** rode `perf annotate` em um
        programa e observe o Assembly x86 anotado com porcentagem de tempo.
        Procure instruções AVX (começam com `v`), que são extensões SIMD da ISA.

### Tópico 5 — Assembly

Conectar Assembly à prática do laboratório é mais sutil. Sugestões:

- **Mini-experimento F — O custo de uma instrução cara:** escreva dois
        programas em C: um com loop simples e outro com `__builtin_popcount`
        (instrução POPCNT do x86). Compile com `-O2` e rode `perf stat`.
        A diferença em IPC mostra o ganho de uma instrução especializada.
- **Mini-experimento G — Assembly inline:** em C, use `__asm__ volatile("...")` para forçar uma instrução específica. Compare a contagem de ciclos do `perf stat` com a versão sem inline.

### Tópico 6 — Monociclo vs Multiciclo

Esses são modelos **didáticos** — CPUs reais não são puramente monociclo nem multiciclo. Sugestões:

- **Mini-experimento H — Simulando no MARS:** o MARS tem dois modos de
        execução visual. Use o modo passo-a-passo para ver instruções MIPS sendo
        executadas em estágios (similar ao multiciclo).
        Conte quantos "ciclos" cada instrução leva.
- **Mini-experimento I — A evolução até o pipeline:** desenhe, em uma folha ou editor, três diagramas temporais para a sequência `add → sub → and`:
  1. Em monociclo (uma instrução de cada vez, ciclo longo)
  2. Em multiciclo (instruções divididas em etapas, ciclo curto, sem sobreposição)
  3. Em pipeline (etapas sobrepostas)

        Calcule quantos ciclos de clock cada modelo leva.
        A diferença é dramática e representa o ganho histórico do pipeline.

---

## Como Estudar para a Prova Usando este Material

Sugestão de roteiro de revisão na semana antes da avaliação:

1. **Releia este documento** (15 min) — entenda o mapa geral.
2. **Para cada tópico da ementa, abra o bloco correspondente** e revise:
   - O box " Ponte com a teoria que vocês já viram"
   - As perguntas reflexivas (são candidatas naturais a questões de prova!)
   - A tabela de resultados que vocês preencheram no relatório
3. **Releia o glossário do htop/top** ([`docs/referencia-htop-top/`](../referencia-htop-top/README.md)) — várias colunas (`VIRT`, `RES`, estados de processo) cobrem tópicos da ementa.
4. **Revise as conexões entre blocos** — as seções "Conexão com o próximo bloco" no final de cada README. Elas são o fio narrativo da disciplina.

> **Truque pedagógico:** se vocês souberem **explicar para um colega** cada um dos 14 tópicos usando um experimento concreto que fizeram no lab, vocês dominaram a matéria. Essa é a meta.

---

## Mapa Conceitual: Do Transistor à Aplicação

Para fixar, aqui está o "fio condutor" da disciplina, conectando os 14 tópicos numa única narrativa vertical:

```text
APLICAÇÃO (programa do usuário)
        ↓ é compilado para
ASSEMBLY                                          ← Tópico 5
        ↓ obedece ao contrato
ISA (MIPS, x86, ARM)                              ← Tópico 4
        ↓ é executada no
PROCESSADOR
        │
        ├─ MONOCICLO (didático)                   ← Tópico 6
        ├─ MULTICICLO (didático)                  ← Tópico 6
        └─ PIPELINE (real)                        ← Tópico 7
                ↓ acessa dados pela
HIERARQUIA DE MEMÓRIA                             ← Tópico 9
        │
        ├─ CACHE L1, L2, L3 (SRAM)                ← Tópico 9 + Tópicos 1, 2, 3 (lógica)
        ├─ MEMÓRIA PRINCIPAL (DRAM)               ← Tópico 10
        └─ ARMAZENAMENTO (SSD, HDD)               ← Tópico 11
                ↓ unificados pela abstração
MEMÓRIA VIRTUAL (paginação)                       ← Tópico 12
        ↓ acessada via
BARRAMENTO (PCIe, DDR)                            ← Tópico 13
        ↓ comunica eventos via
EXCEÇÕES, INTERRUPÇÕES, I/O                       ← Tópico 8
        ↓ tudo isso evolui em direção a
ARQUITETURAS AVANÇADAS                            ← Tópico 14
(multicore, SIMD, NUMA, GPU, aceleradores)
```

> **A grande síntese:** a disciplina é a história de
> **como abstrair sem perder eficiência**.
> Cada tópico é uma camada que esconde complexidade da camada acima,
> e cada uma cobra um preço.
> Vocês passaram um semestre aprendendo a **enxergar esses preços**
> em medições concretas. Esse é o legado da matéria.

---

## Referências para Estudo Aprofundado

- **Patterson, D. A., & Hennessy, J. L.** *Computer Organization and Design — The Hardware/Software Interface* (MIPS Edition). É o livro-base e cobre os 14 tópicos com profundidade.
- **Stallings, W.** *Computer Organization and Architecture*. Visão complementar com mais ênfase em arquiteturas reais.
- **Bryant, R. E., & O'Hallaron, D. R.** *Computer Systems: A Programmer's Perspective* (CSAPP). Excelente para conectar Assembly e desempenho.
- **Brendan Gregg** — `brendangregg.com`. Site de referência mundial em análise de performance de sistemas Linux. Use para o projeto final.
- **Manuais oficiais** — Intel SDM (Software Developer Manual) e AMD APM. São densos, mas a fonte primária da ISA x86 real.
