# Aula 2 — A Pirâmide da Memória: Hierarquia de Caches

## Descrição

Este bloco materializa, **com números medidos pelos próprios alunos**, a famosa "pirâmide de memória". Eles descobrem na prática que a latência de acesso cresce em **ordens de grandeza** entre os níveis — e essa descoberta é o gancho para entender por que cache miss é tão caro e por que algoritmos cache-friendly importam.

## Objetivos de Aprendizagem

Ao final deste bloco, o aluno deve ser capaz de:
- Construir a tabela de latências dos níveis L1, L2, L3 e RAM da máquina
- Distinguir cache miss e cache hit em termos de penalidade de tempo
- Reconhecer localidade temporal e espacial em código
- Justificar por que um algoritmo "cache-aware" pode ser ordens de magnitude mais rápido

---

## Passo a Passo da Prática

### Atividade 2.1 — Medindo a latência por nível (10 min)

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

### Atividade 2.2 — O experimento da localidade (10 min)

Use o script [`scripts/teste_localidade.py`](./scripts/teste_localidade.py) deste repositório:

```bash
python3 scripts/teste_localidade.py
```

Este script implementa **dois loops que acessam a mesma quantidade de dados**, mas em padrões de acesso diferentes:
- **Loop A:** percorre uma matriz linha por linha (acesso sequencial, amigo do cache)
- **Loop B:** percorre a mesma matriz coluna por coluna (acesso "saltando", hostil ao cache)

Os alunos vão observar que o Loop B é **muito mais lento**, mesmo executando rigorosamente a mesma quantidade de operações.

> **Pedagogicamente forte:** este é um dos experimentos que mais "abrem a cabeça" do aluno — a diferença de tempo é tão gritante que dispensa explicação teórica adicional.

### Tabela de Resultados (preencher no relatório)

| Nível | Tamanho típico | Latência medida (ns) | Largura de banda (GB/s) |
|-------|----------------|----------------------|--------------------------|
| L1 | ~32-64 KB | | |
| L2 | ~256 KB – 1 MB | | |
| L3 | ~4-32 MB | | |
| RAM | GBs | | |

#### Tabela do experimento de localidade

| Padrão de acesso | Tempo (s) | Operações por segundo | Razão (B/A) |
|------------------|-----------|------------------------|-------------|
| Linha por linha (sequencial) | | | 1.00 |
| Coluna por coluna (saltando) | | | |

---

## Perguntas Reflexivas

1. Por que a latência cresce **em ordens de grandeza** (e não linearmente) ao descer a pirâmide? Que princípio físico justifica isso?
2. Um vetor de **100 MB** cabe em qual nível da hierarquia da sua máquina? E um vetor de **200 KB**? Qual deles você espera que rode mais rápido em um loop sequencial — e por quê?
3. Defina **localidade temporal** e **localidade espacial** com suas palavras. Dê um exemplo de código C que explore bem cada uma.
4. No experimento de localidade, **a quantidade de operações foi a mesma** nos dois loops. Por que o tempo foi diferente? O que isso revela sobre o que de fato custa ao processador?
5. Lembrando do Bloco 1: quando dois núcleos rodam simultaneamente, eles **disputam o L3 compartilhado**. Como isso afeta o fator de escala paralelo que vocês mediram?

---

## Conexão com o próximo bloco

> *"Vimos que o L3 já é caro. Agora pensem: quando os dados saem da RAM e vão para o **SSD**, a penalidade aumenta em mais 4-5 ordens de grandeza. É exatamente isso que acontece no swap — e é por isso que falta de RAM destrói o desempenho. No próximo bloco, vamos medir essa penalidade na prática."*

## Scripts deste bloco

- [`scripts/teste_localidade.py`](./scripts/teste_localidade.py) — demonstra impacto de localidade espacial

---

[← Aula 1](../aula1-anatomia-cpu-pipeline-multicore/README.md) | [Índice](../../README.md) | **Aula 2** | [Aula 3 →](../aula3-ram-armazenamento-memoria-virtual/README.md)
