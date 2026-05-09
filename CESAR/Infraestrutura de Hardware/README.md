# Laboratório de Infraestrutura de Hardware

## Sobre este material

Este repositório contém o roteiro de uma **aula prática única de 2h**
dividida em 5 blocos sequenciais. A narrativa é investigativa: você parte da
pergunta-âncora("por que dois computadores aparentemente similares têm
desempenhos tão diferentes?") e usa **benchmarking + ferramentas de
diagnóstico** em Windows e Linux para responder, integrando os 14 tópicos da
ementa:

| #   | Tópico                        | Onde aparece                                         |
| --- | ----------------------------- | ---------------------------------------------------- |
| 1   | Multiplexador                 | Aula 2 (cache associativo) + bridge                  |
| 2   | Demultiplexador               | Aula 2 (decode de endereço) + bridge                 |
| 3   | Flip-Flop                     | Aula 2 (células SRAM) + bridge                       |
| 4   | ISA / MIPS                    | Aula 1 (extensões x86) + atividade opcional MARS     |
| 5   | Assembly                      | Aula 1 (atividade opcional) + Aula 5 (perf annotate) |
| 6   | Monociclo / Multiciclo        | Aula 1 (box de teoria)                               |
| 7   | Pipeline                      | Aula 1 + Aula 5                                      |
| 8   | Exceções, Interrupções, I/O   | Aula 4                                               |
| 9   | Hierarquia de Memória e Cache | Aula 2                                               |
| 10  | Memória Principal (RAM)       | Aula 2 e Aula 3                                      |
| 11  | Armazenamento                 | Aula 3                                               |
| 12  | Memória Virtual               | Aula 3                                               |
| 13  | Barramento                    | Aula 4                                               |
| 14  | Arquitetura Avançada          | Aula 1 (multicore) + Aula 5 (SIMD, NUMA, MESI)       |

> **Para o mapa completo** com sugestões de mini-experimentos opcionais para
> os tópicos mais conceituais (lógica digital, monociclo/multiciclo e ISA),
> veja [`docs/teoria-pratica-bridge/`](docs/teoria-pratica-bridge/README.md).
> Esse documento é o **guia de revisão antes da prova**.

## Estrutura

```text
docs/
├── analise-performance-algoritmos/             # Apoio — análise complementar
├── aula1-anatomia-cpu-pipeline-multicore/      # Bloco 1 — 00 min
├── aula2-hierarquia-memoria-caches/            # Bloco 2 — 00 min
├── aula3-ram-armazenamento-memoria-virtual/    # Bloco 3 — 00 min
├── aula4-barramentos-io-interrupcoes/          # Bloco 4 — 00 min
├── aula5-sintese-benchmark-integrado/          # Bloco 5 — 00 min
├── referencia-htop-top/                        #  Glossário de consulta
└── teoria-pratica-bridge/                      #  Mapa teoria ↔ prática
relatorio-template/
└── relatorio.md                                # Modelo de entrega
```

### Documentos das Aulas

| Bloco | Documento                                                                                        | Duração |
| ----- | ------------------------------------------------------------------------------------------------ | ------- |
| 1     | [Anatomia da CPU, Pipeline e Multicore](./docs/aula1-anatomia-cpu-pipeline-multicore/README.md)  | 00 min  |
| 2     | [Hierarquia de Memória e Caches](./docs/aula2-hierarquia-memoria-caches/README.md)               | 00 min  |
| 3     | [RAM, Armazenamento e Memória Virtual](./docs/aula3-ram-armazenamento-memoria-virtual/README.md) | 00 min  |
| 4     | [Barramentos, I/O e Interrupções](./docs/aula4-barramentos-io-interrupcoes/README.md)            | 00 min  |
| 5     | [Síntese e Benchmark Integrado](./docs/aula5-sintese-benchmark-integrado/README.md)              | 00 min  |
| Apoio | [Análise de Performance de Algoritmos](./docs/analise-performance-algoritmos/README.md)          | Extra   |
| -     | [Modelo de Relatório](./relatorio-template/relatorio.md)                                         | -       |

> **Materiais de consulta:**
>
> - [`docs/referencia-htop-top/`](docs/referencia-htop-top/README.md) — glossário das colunas do `htop`/`top` (PID, PRI, NI, VIRT, RES, SHR, S, etc.).
> - [`docs/teoria-pratica-bridge/`](docs/teoria-pratica-bridge/README.md) — mapa explícito entre cada tópico da ementa e os experimentos do laboratório, com sugestões de mini-experimentos opcionais.

Cada pasta contém um `README.md` com o passo a passo (Windows + Linux), além de scripts auxiliares na subpasta `scripts/`.

## Cronograma de Referência (28/04 — 12/06)

| Semana | Datas         | Conteúdo                                                                                              |
| ------ | ------------- | ----------------------------------------------------------------------------------------------------- |
| 1      | 28/04 e 30/04 | Apresentação + revisão de Von Neumann e ciclo de instrução. **Tarefa de casa: instalar ferramentas.** |
| 2      | 05/05 e 07/05 | Pipeline e multicore (teoria)                                                                         |
| 3      | 12/05 e 14/05 | Hierarquia de memória e caches (teoria)                                                               |
| 4      | 19/05 e 21/05 | Memória virtual + **LAB COMPLETO**                                                                    |
| 5      | 26/05 e 28/05 | Barramentos, I/O, interrupções (revisão pós-lab)                                                      |
| 6      | 02/06 e 04/06 | Arquiteturas avançadas (SIMD, GPU, NUMA)                                                              |
| 7      | 09/06 e 11/06 | Apresentações finais                                                                                  |

> **Variante:** se preferir dividir o lab em dois encontros de 1h, use Aulas 1+2 em 14/05 e Aulas 3+4+5 em 28/05.

## Pré-requisitos: ferramentas a instalar

### Windows (todas gratuitas / trial)

- **CPU-Z** — identificação de CPU/RAM/chipset
- **HWiNFO64** — sensores e barramento PCIe
- **CrystalDiskMark** — benchmark de armazenamento
- **AIDA64 Trial** — cache & memory benchmark (ou Sandra Lite)
- **Cinebench R23** — benchmark de CPU single/multi-core
- **LatencyMon** — análise de ISRs/DPCs
- **Process Explorer** (Sysinternals) — análise detalhada de processos
- **RAMMap** (Sysinternals) — mapa de uso de memória
- **Python 3.10+** com `numpy` e `psutil` (para os scripts deste repositório)

### Linux (Ubuntu/Debian — pacotes apt)

```bash
sudo apt update
sudo apt install -y hwloc lstopo dmidecode sysbench fio stress-ng \
  htop linux-tools-common linux-tools-generic numactl pciutils \
  hdparm lshw build-essential python3-pip
pip3 install psutil numpy
```

## Dinâmica da Prática

Trabalhe **individualmente** e mantenha um **caderno de laboratório digital** (use o `relatorio-template/relatorio.md`) para registrar:

- Capturas de tela das ferramentas
- Tabelas de medições
- Respostas às perguntas reflexivas

## Avaliação

A definir

## Autor

- **Prof. Roni Maciel**
- LinkedIn: [@0x03c1](https://linkedin.com/in/0x03c1)
