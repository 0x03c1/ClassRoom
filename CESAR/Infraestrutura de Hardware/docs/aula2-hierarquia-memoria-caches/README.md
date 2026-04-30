# Aula 2 — A Pirâmide da Memória: Hierarquia de Caches

Foco em cache L1, L2, L3, latência e localidade.

## Navegação

- [Voltar ao índice do laboratório](../../README.md)
- [Aula anterior: Anatomia da CPU, Pipeline e Multicore](../aula1-anatomia-cpu-pipeline-multicore/README.md)
- [Próxima aula: RAM, Armazenamento e Memória Virtual](../aula3-ram-armazenamento-memoria-virtual/README.md)

## Ponte com a teoria

Neste bloco, você relaciona medições reais com conceitos de SRAM, DRAM e
localidade. O objetivo é observar que a diferença entre L1, L2, L3 e RAM
não é apenas teórica: ela aparece claramente nos tempos de acesso.

## Descrição

Este bloco transforma a "pirâmide de memória" em números medidos na sua máquina. Ao final, você entende por que cache miss custa caro e por que padrões de acesso cache-friendly mudam o desempenho.

## Objetivos de Aprendizagem

Ao final deste bloco, o aluno deve ser capaz de:

- Construir a tabela de latências dos níveis L1, L2, L3 e RAM da máquina
- Distinguir cache miss e cache hit em termos de penalidade de tempo
- Reconhecer localidade temporal e espacial em código
- Justificar por que um algoritmo "cache-aware" pode ser ordens de magnitude mais rápido

---

## Passo a Passo da Prática

### Atividade 2.1 — Medindo a latência por nível

#### No Windows — AIDA64 Cache & Memory Benchmark

1. Abra o **AIDA64 Trial**.
2. Vá em **Tools → Cache & Memory Benchmark**.
3. Clique em "Start Benchmark". O teste leva ~2 min.
4. Anote os valores de **Read / Write / Copy / Latency** para L1, L2, L3 e RAM.

> Alternativa caso AIDA64 não esteja disponível: **SiSoftware Sandra Lite** → Benchmarks → Cache & Memory Bandwidth.

#### No Linux — Sysbench

```bash
# Bloco pequeno (cabe em L1)
sysbench memory --memory-block-size=1K --memory-total-size=10G run | grep "transferred"

# Bloco médio (cabe em L2/L3)
sysbench memory --memory-block-size=512K --memory-total-size=10G run | grep "transferred"

# Bloco grande (sai pra RAM)
sysbench memory --memory-block-size=64M --memory-total-size=10G run | grep "transferred"
```

> Observe como a largura de banda **cai** conforme o bloco cresce — é o efeito da hierarquia de memória se manifestando.

#### No Linux — perf (mais profundo, opcional)

```bash
# Conta cache misses durante a execução de um comando
sudo perf stat -e cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses \
  ls -laR /usr/ > /dev/null
```

A taxa `cache-misses / cache-references` mostra quanta pressão o programa colocou sobre o cache.

### Atividade 2.2 — O experimento da localidade

Use o script [`scripts/teste_localidade.py`](./scripts/teste_localidade.py) deste repositório:

```bash
python3 scripts/teste_localidade.py
```

Este script implementa **dois loops que acessam a mesma quantidade de dados**, mas em padrões de acesso diferentes:

- **Loop A:** percorre uma matriz linha por linha (acesso sequencial, amigo do cache)
- **Loop B:** percorre a mesma matriz coluna por coluna (acesso "saltando", hostil ao cache)

Você vai observar que o Loop B é **muito mais lento**, mesmo executando rigorosamente a mesma quantidade de operações.

> Interpretação esperada: a diferença de tempo aparece mesmo com o mesmo número de operações, porque o padrão de acesso à memória impacta diretamente o cache.

### Tabela de Resultados (preencher no relatório)

| Nível | Tamanho típico | Latência medida (ns) | Largura de banda (GB/s) |
| ----- | -------------- | -------------------- | ----------------------- |
| L1    | ~32-64 KB      |                      |                         |
| L2    | ~256 KB – 1 MB |                      |                         |
| L3    | ~4-32 MB       |                      |                         |
| RAM   | GBs            |                      |                         |

#### Tabela do experimento de localidade

| Padrão de acesso             | Tempo (s) | Operações por segundo | Razão (B/A) |
| ---------------------------- | --------- | --------------------- | ----------- |
| Linha por linha (sequencial) |           |                       | 1.00        |
| Coluna por coluna (saltando) |           |                       |             |

---

## Perguntas Reflexivas

1. Por que a latência cresce **em ordens de grandeza** (e não linearmente) ao descer a pirâmide? Que princípio físico justifica isso?
2. Um vetor de **100 MB** cabe em qual nível da hierarquia da sua máquina? E um vetor de **200 KB**? Qual deles você espera que rode mais rápido em um loop sequencial — e por quê?
3. Defina **localidade temporal** e **localidade espacial** com suas palavras. Dê um exemplo de código C que explore bem cada uma.
4. No experimento de localidade, **a quantidade de operações foi a mesma** nos dois loops. Por que o tempo foi diferente? O que isso revela sobre o que de fato custa ao processador?
5. Lembrando do Bloco 1: quando dois núcleos rodam simultaneamente, eles **disputam o L3 compartilhado**. Como isso afeta o fator de escala paralelo que você mediu?

---

## Conexão com o próximo bloco

> No próximo bloco, você vai comparar RAM e armazenamento e medir o impacto da paginação quando o sistema começa a usar swap.

## Scripts deste bloco

- [`scripts/teste_localidade.py`](./scripts/teste_localidade.py) — demonstra impacto de localidade espacial
