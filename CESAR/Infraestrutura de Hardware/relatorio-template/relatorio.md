# Relatório de Laboratório — Infraestrutura de Hardware

> Disciplina: Infraestrutura de Hardware | Prof. Ronierison Maciel
> Data do laboratório: ___/___/______
> Dupla: __________________________ e__________________________
> Máquina utilizada: ____________________________________

---

## 0. Hipóteses iniciais

Antes do laboratório, listamos cinco métricas que usaríamos para comparar duas máquinas:

1.
2.
3.
4.
5.

Hipóteses levantadas em sala (registrar para revisitar no Bloco 5):

- [ ]
- [ ]
- [ ]

---

## 1. Anatomia da CPU (Bloco 1)

### Configuração identificada

| Métrica                           | Valor |
| --------------------------------- | ----- |
| Modelo da CPU                     |       |
| Núcleos físicos / Threads lógicas | /     |
| Frequência base (GHz)             |       |
| Frequência turbo observada (GHz)  |       |
| Cache L1 / L2 / L3                | / /   |
| Instruction sets relevantes       |       |

### Benchmark de paralelismo

| Métrica                        | Valor |
| ------------------------------ | ----- |
| Cinebench Single Core          |       |
| Cinebench Multi Core           |       |
| Fator de escala (Multi/Single) |       |
| Fator ideal (= nº de threads)  |       |
| Eficiência paralela (%)        |       |

__Screenshots colados:__ _(CPU-Z, HWiNFO, Cinebench)_

### Respostas às perguntas reflexivas (Bloco 2)

1. __Por que o fator de escala é menor que o número de threads?__

2. __Por que a carga "pula" entre núcleos no teste single-core?__

3. __Pipeline com N estágios — ganho ideal e o que quebra?__

4. __Tradução entre GHz e pontos via CPI?__

---

## 2. Hierarquia de Memória (Bloco 2)

### Latência por nível

| Nível | Tamanho | Latência (ns) | Banda (GB/s) |
| ----- | ------- | ------------- | ------------ |
| L1    |         |               |              |
| L2    |         |               |              |
| L3    |         |               |              |
| RAM   |         |               |              |

### Experimento de localidade

| Padrão                       | Tempo (s) | Razão |
| ---------------------------- | --------- | ----- |
| Linha por linha (sequencial) |           | 1.00  |
| Coluna por coluna (saltando) |           |       |

### Respostas às perguntas reflexivas (Bloco 3)

1. __Por que a latência cresce em ordens de grandeza?__

2. __Vetor de 100 MB vs 200 KB — qual é mais rápido em loop sequencial?__

3. __Localidade temporal e espacial — exemplos em código:__

4. __Mesmo nº de operações, tempos diferentes — por quê?__

5. __Disputa por L3 e fator de escala paralelo:__

---

## 3. RAM, Armazenamento e Memória Virtual (Bloco 3)

### Hierarquia completa

| Componente           | Banda medida | Latência típica |
| -------------------- | ------------ | --------------- |
| L1                   |              | ~1 ns           |
| L3                   |              | ~10-15 ns       |
| RAM                  |              | ~70-100 ns      |
| SSD NVMe SEQ1M       |              | ~50-200 µs      |
| SSD NVMe RND4K       |              | ~50-200 µs      |
| SSD SATA (se houver) |              | ~80-300 µs      |
| HDD (se houver)      |              | ~5-15 ms        |

### Memória virtual de um processo escolhido

Processo: _________________

| Métrica               | Valor |
| --------------------- | ----- |
| Working Set / VmRSS   |       |
| Private Bytes         |       |
| Virtual Size / VmSize |       |

### Pressão de memória — observações

_(O que aconteceu com `free`/`vmstat` quando aplicamos pressão? Houve swap?)_

### Respostas às perguntas reflexivas

1. __Virtual Size > RAM física — o que isso revela?__

2. __Penalidade do swap em ordens de grandeza:__

3. __Page fault — minor vs major:__

4. __Por que `vm.swappiness=1` em servidores de banco?__

5. __RND4K vs SEQ1M — por que tão diferente?__

6. __"NVMe é melhor que mais RAM" — refute com dados:__

---

## 4. Barramentos, I/O e Interrupções (Bloco 4)

### PCIe identificado

| Dispositivo | Geração | Largura | Banda teórica | Banda real medida |
| ----------- | ------- | ------- | ------------- | ----------------- |
| SSD NVMe    |         |         |               |                   |
| GPU         |         |         |               |                   |
| Rede        |         |         |               |                   |

### Interrupções observadas

#### Quais IRQs subiram com cada ação?

- Mexer mouse: IRQ ____
- Digitar: IRQ ____
- Ping: IRQ ____
- Cópia de arquivo: IRQ ____

### Respostas às perguntas reflexivas (Bloco 4)

1. __PCIe 4.0 x16 vs RAM — onde fica o gargalo numa GPU?__

2. __Por que existem interrupções? E por que polling é pior?__

3. __Quando polling é melhor que interrupção?__

4. __Cadeia tecla-pressionada → tela:__

5. __NVMe 7000 MB/s em PCIe 3.0 x4 — qual o teto real?__

6. __DMA — o que é e como reduz carga da CPU?__

---

## 5. Síntese e Resolução do Mistério (Bloco 5)

### Workload escolhido

Workload: __________________________________________

### Métricas integradas

| Subsistema      | Métrica          | Valor medido |
| --------------- | ---------------- | ------------ |
| CPU             | IPC              |              |
| Cache           | Taxa de miss (%) |              |
| Memória virtual | Page faults / s  |              |
| Armazenamento   | Throughput       |              |
| Multicore       | Threads em uso   |              |
| PCIe            | Saturação        |              |

### Hipóteses revisitadas

#### Classificação das hipóteses do Bloco 0

| Hipótese inicial | Status                          | Evidência |
| ---------------- | ------------------------------- | --------- |
|                  | Confirmada / Refutada / Parcial |           |
|                  | Confirmada / Refutada / Parcial |           |
|                  | Confirmada / Refutada / Parcial |           |

### Perguntas-síntese (valem nota)

__1. Qual é o gargalo dominante da minha máquina?__ Justifique com 3 métricas medidas.

__2. Onde investir R$ 500 em upgrade?__ Defesa técnica:

__3. Xeon 32C/2.5GHz vs i9 8C/5.5GHz — quem ganha em qual cenário?__ Use Lei de Amdahl:

#### 4. Como cada conceito afetou meu workload? (uma frase cada)

- Pipeline e ILP:
- Cache:
- Memória virtual:
- PCIe / barramento:
- Multicore:

---

## 6. Análise Crítica Comparativa (1 página)

### Orientação

Compare a máquina do laboratório com sua máquina pessoal de casa.
Use métricas. Onde estão as maiores diferenças? Por quê?
O que o usuário comum não percebe?

---

## Referências consultadas

- Patterson, D. A., & Hennessy, J. L. _Computer Organization and Design_.
- Stallings, W. _Computer Organization and Architecture_.
- Documentação das ferramentas usadas (citar).
