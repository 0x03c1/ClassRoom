# Relatório do Laboratório — Infraestrutura de Hardware

> **Como usar:** copie este arquivo para a sua pasta de trabalho, renomeie
> para `relatorio_<seu_nome>.md` e preencha à medida que executa as
> atividades. A ideia é que ao final do laboratório você tenha um
> documento técnico mostrando como sua máquina se comporta.

---

## Identificação

- **Dupla / trio:** ___________________________________________________
- **Máquina(s) usadas:** ______________________________________________
- **Sistema operacional:** ____________________________________________
- **Data:** ___ / ___ / 20___

---

## Bloco 0 — Hipóteses iniciais

> *Antes* de qualquer medição, registre **5 métricas** que você usaria
> para comparar duas máquinas. Volte a esta lista no fim do laboratório
> e veja o que mudou no seu raciocínio.

1.
2.
3.
4.
5.

---

## Bloco 1 — Anatomia da CPU (Aula 1)

| Métrica                                  | Valor   |
| ---------------------------------------- | ------- |
| Modelo da CPU                            |         |
| Arquitetura (x86_64 / aarch64 / ...)     |         |
| Núcleos físicos                          |         |
| Threads lógicas (SMT/HT)                 |         |
| Frequência base / boost                  |         |
| L1d / L1i / L2 / L3                      |         |
| Conjunto SIMD mais recente               |         |
| Cinebench R23 — single                   |         |
| Cinebench R23 — multi                    |         |
| Fator de escala (multi/single)           |         |
| Speedup observado em `teste_paralelismo` |         |

**Observações / surpresas:**

```
(o fator de escala bate com o número de threads? Se não, por quê?)
```

---

## Bloco 2 — Hierarquia de Memória (Aula 2)

| Tamanho do array | Vazão (`mbw -t0`) |
| ---------------- | ----------------- |
| 16 MiB           |                   |
| 128 MiB          |                   |
| 1024 MiB         |                   |

| `teste_localidade.py` | Tempo (s)  |
| --------------------- | ---------- |
| Loop A (linhas)       |            |
| Loop B (colunas)      |            |
| Razão B/A             |            |

**Observações:**

```
(em qual tamanho de array a vazão começa a cair? Bate com o L3 da CPU?)
```

---

## Bloco 3 — RAM e Memória Virtual (Aula 3)

| Métrica                          | Valor |
| -------------------------------- | ----- |
| RAM total                        |       |
| RAM disponível em repouso        |       |
| Swap total / em uso              |       |
| `fio` seq read (MB/s)            |       |
| `fio` random read 4k (IOPS)      |       |
| Variação de `si`/`so` durante stress-ng |       |

**Anote o nome do navegador escolhido para `/proc/$PID/status`:** _____________

**Observações:**

```
(o que acontece com o navegador conforme stress-ng pressiona a RAM?)
```

---

## Bloco 4 — Barramentos, I/O e Interrupções (Aula 4)

| Dispositivo PCIe       | LnkCap (geração × largura) | LnkSta |
| ---------------------- | -------------------------- | ------ |
| Controlador NVMe       |                            |        |
| GPU                    |                            |        |
| Outro (ex: rede 10G)   |                            |        |

**Top 3 IRQs durante o `dd`:**

```
(saída de cat /proc/interrupts antes e depois)
```

---

## Bloco 5 — Síntese e Benchmark Integrado (Aula 5)

| Workload (`workload_completo.py`)        | Resultado |
| ---------------------------------------- | --------- |
| CPU-bound — multiplicações/s             |           |
| Memory-bound — bandwidth (GB/s)          |           |
| I/O-bound — throughput (MB/s)            |           |
| Cores >50% no fim                        |           |

**Gargalo dominante (segundo o script):** _________________________________

**Você concorda? Justifique:**

```
```

---

## Bloco final — Volte ao Bloco 0

Reabra a sua lista do Bloco 0. Quais métricas você manteria? Quais
trocaria? Por quê?

```
```

---

## Anexos

- Saída de `info_cpu.sh`: `info_cpu_AAAAMMDD_HHMMSS.txt`
- Gráfico de complexidade: `complexidade_empirica.png`
- Comparação de stacks: `comparacao_stacks.png`
- Flame graph (se feito): `flame.svg`
