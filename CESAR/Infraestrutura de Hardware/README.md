# Laboratório de Infraestrutura de Hardware

> Roteiro prático para o laboratório — Disciplina de Infraestrutura de Hardware
> CESAR School / 3º período — Ciência da Computação
> Prof. Roni Maciel

## Sobre este material

Este repositório contém o roteiro de uma **aula prática de 2h** dividida em 5 blocos sequenciais. A narrativa é investigativa: os alunos partem de uma pergunta-âncora ("por que dois computadores aparentemente similares têm desempenhos tão diferentes?") e usam **benchmarking + ferramentas de diagnóstico** em Windows e Linux para responder, integrando os 7 tópicos da ementa:

1. Pipeline (visão lógica)
2. Mecanismos de Interrupção e I/O
3. Hierarquia de Memória (Cache L1, L2, L3)
4. RAM vs Armazenamento (SSD NVMe vs SATA)
5. Memória Virtual e paginação
6. Barramentos (PCIe, largura de banda)
7. Multicore e paralelismo

## Estrutura

```
docs/
├── aula1-anatomia-cpu-pipeline-multicore/      # Bloco 1 — 00 min
├── aula2-hierarquia-memoria-caches/            # Bloco 2 — 00 min
├── aula3-ram-armazenamento-memoria-virtual/    # Bloco 3 — 00 min
├── aula4-barramentos-io-interrupcoes/          # Bloco 4 — 00 min
└── aula5-sintese-benchmark-integrado/          # Bloco 5 — 00 min
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
| —     | [Modelo de Relatório](./relatorio-template/relatorio.md)                                         | —       |

Cada pasta contém um `README.md` com o passo a passo (Windows + Linux), além de scripts auxiliares na subpasta `scripts/`.

## Cronograma de Referência (00/00 — 00/00)

| Semana | Datas         | Conteúdo                                                                                              |
| ------ | ------------- | ----------------------------------------------------------------------------------------------------- |
| 1      | 00/00 e 00/00 | Apresentação + revisão de Von Neumann e ciclo de instrução. **Tarefa de casa: instalar ferramentas.** |
| 2      | 00/00 e 00/00 | Pipeline e multicore (teoria)                                                                         |
| 3      | 00/00 e 00/00 | Hierarquia de memória e caches (teoria)                                                               |
| 4      | 00/00 e 00/00 | Memória virtual + **LAB COMPLETO**                                                      |
| 5      | 00/00 e 00/00 | Barramentos, I/O, interrupções (revisão pós-lab)                                                      |
| 6      | 00/00 e 00/00 | Arquiteturas avançadas (SIMD, GPU, NUMA)                                                              |
| 7      | 00/00 e 00/00 | Apresentações finais                                                                                  |

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

## Cenário Pedagógico

Os alunos formam **duplas** e cada dupla recebe uma máquina. Idealmente, organize as duplas para que as configurações variem entre as máquinas do laboratório (ex: máquinas com SSDs diferentes, RAMs em quantidades distintas, gerações de CPU diversas). Isso enriquece a discussão final do Bloco 5, quando os resultados são comparados entre grupos.

Cada dupla deve manter um **caderno de laboratório digital** (use o `relatorio-template/relatorio.md`) onde registra:
- Capturas de tela das ferramentas
- Tabelas de medições
- Respostas às perguntas reflexivas

## Avaliação

Relatório em dupla (entrega 1 semana após o lab) contendo:
- Tabelas de todos os blocos preenchidas
- Respostas às 4 perguntas-síntese do Bloco 5
- Análise crítica de 1 página: comparação entre a máquina do laboratório e uma máquina pessoal

## Autor

**Prof. Roni Maciel** — Ciência da Computação | CESAR School
LinkedIn: [@0x03c1](https://linkedin.com/in/0x03c1)
