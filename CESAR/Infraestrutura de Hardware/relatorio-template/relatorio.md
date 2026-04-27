# Relatório de Laboratório — Infraestrutura de Hardware

> Disciplina: Infraestrutura de Hardware | Prof. Ronierison Maciel
> Data do laboratório: ___/___/______
> Dupla: __________________________ e __________________________
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

| Métrica | Valor |
|---------|-------|
| Modelo da CPU | |
| Núcleos físicos / Threads lógicas | / |
| Frequência base (GHz) | |
| Frequência turbo observada (GHz) | |
| Cache L1 / L2 / L3 | / / |
| Instruction sets relevantes | |

### Benchmark de paralelismo

| Métrica | Valor |
|---------|-------|
| Cinebench Single Core | |
| Cinebench Multi Core | |
| Fator de escala (Multi/Single) | |
| Fator ideal (= nº de threads) | |
| Eficiência paralela (%) | |

**Screenshots colados:** _(CPU-Z, HWiNFO, Cinebench)_

### Respostas às perguntas reflexivas

1. **Por que o fator de escala é menor que o número de threads?**



2. **Por que a carga "pula" entre núcleos no teste single-core?**



3. **Pipeline com N estágios — ganho ideal e o que quebra?**



4. **Tradução entre GHz e pontos via CPI?**



---

## 2. Hierarquia de Memória (Bloco 2)

### Latência por nível

| Nível | Tamanho | Latência (ns) | Banda (GB/s) |
|-------|---------|---------------|---------------|
| L1 | | | |
| L2 | | | |
| L3 | | | |
| RAM | | | |

### Experimento de localidade

| Padrão | Tempo (s) | Razão |
|--------|-----------|--------|
| Linha por linha (sequencial) | | 1.00 |
| Coluna por coluna (saltando) | | |

### Respostas às perguntas reflexivas

1. **Por que a latência cresce em ordens de grandeza?**



2. **Vetor de 100 MB vs 200 KB — qual é mais rápido em loop sequencial?**



3. **Localidade temporal e espacial — exemplos em código:**



4. **Mesmo nº de operações, tempos diferentes — por quê?**



5. **Disputa por L3 e fator de escala paralelo:**



---

## 3. RAM, Armazenamento e Memória Virtual (Bloco 3)

### Hierarquia completa

| Componente | Banda medida | Latência típica |
|------------|---------------|-------------------|
| L1 | | ~1 ns |
| L3 | | ~10-15 ns |
| RAM | | ~70-100 ns |
| SSD NVMe SEQ1M | | ~50-200 µs |
| SSD NVMe RND4K | | ~50-200 µs |
| SSD SATA (se houver) | | ~80-300 µs |
| HDD (se houver) | | ~5-15 ms |

### Memória virtual de um processo escolhido

Processo: _________________

| Métrica | Valor |
|---------|-------|
| Working Set / VmRSS | |
| Private Bytes | |
| Virtual Size / VmSize | |

### Pressão de memória — observações

_(O que aconteceu com `free`/`vmstat` quando aplicamos pressão? Houve swap?)_



### Respostas às perguntas reflexivas

1. **Virtual Size > RAM física — o que isso revela?**



2. **Penalidade do swap em ordens de grandeza:**



3. **Page fault — minor vs major:**



4. **Por que `vm.swappiness=1` em servidores de banco?**



5. **RND4K vs SEQ1M — por que tão diferente?**



6. **"NVMe é melhor que mais RAM" — refute com dados:**



---

## 4. Barramentos, I/O e Interrupções (Bloco 4)

### PCIe identificado

| Dispositivo | Geração | Largura | Banda teórica | Banda real medida |
|--------------|----------|----------|----------------|---------------------|
| SSD NVMe | | | | |
| GPU | | | | |
| Rede | | | | |

### Interrupções observadas

_(Quais IRQs subiram com cada ação?)_

- Mexer mouse: IRQ ____
- Digitar: IRQ ____
- Ping: IRQ ____
- Cópia de arquivo: IRQ ____

### Respostas às perguntas reflexivas

1. **PCIe 4.0 x16 vs RAM — onde fica o gargalo numa GPU?**



2. **Por que existem interrupções? E por que polling é pior?**



3. **Quando polling é melhor que interrupção?**



4. **Cadeia tecla-pressionada → tela:**



5. **NVMe 7000 MB/s em PCIe 3.0 x4 — qual o teto real?**



6. **DMA — o que é e como reduz carga da CPU?**



---

## 5. Síntese e Resolução do Mistério (Bloco 5)

### Workload escolhido

Workload: __________________________________________

### Métricas integradas

| Subsistema | Métrica | Valor medido |
|-------------|----------|---------------|
| CPU | IPC | |
| Cache | Taxa de miss (%) | |
| Memória virtual | Page faults / s | |
| Armazenamento | Throughput | |
| Multicore | Threads em uso | |
| PCIe | Saturação | |

### Hipóteses revisitadas

_(Voltamos à lista do Bloco 0 e classificamos cada hipótese)_

| Hipótese inicial | Status | Evidência |
|-------------------|---------|------------|
| | // | |
| | // | |
| | // | |

### Perguntas-síntese (valem nota)

**1. Qual é o gargalo dominante da minha máquina?** Justifique com 3 métricas medidas.



**2. Onde investir R$ 500 em upgrade?** Defesa técnica:



**3. Xeon 32C/2.5GHz vs i9 8C/5.5GHz — quem ganha em qual cenário?** Use Lei de Amdahl:



**4. Como cada conceito afetou meu workload? (uma frase cada)**

- Pipeline e ILP:
- Cache:
- Memória virtual:
- PCIe / barramento:
- Multicore:

---

## 6. Análise Crítica Comparativa (1 página)

_(Compare a máquina do laboratório com sua máquina pessoal de casa. Use métricas. Onde estão as maiores diferenças? Por quê? O que o usuário comum não percebe?)_




---

## Referências consultadas

- Patterson, D. A., & Hennessy, J. L. *Computer Organization and Design*.
- Stallings, W. *Computer Organization and Architecture*.
- Documentação das ferramentas usadas (citar).
