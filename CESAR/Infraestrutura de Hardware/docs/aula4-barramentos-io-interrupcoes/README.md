# Aula 4 — Barramentos, I/O e Interrupções

## Descrição

Aqui o aluno vê o **sistema nervoso** da máquina: como os dispositivos conversam com a CPU. Identificamos a versão e largura do PCIe usado pelo SSD e pela GPU, comparamos com a largura de banda da RAM, e observamos as interrupções acontecendo em tempo real conforme periféricos são usados.

## Objetivos de Aprendizagem

Ao final deste bloco, o aluno deve ser capaz de:
- Identificar geração e largura PCIe de um dispositivo (`x4`, `x16`, etc.)
- Calcular largura de banda teórica de PCIe e comparar com RAM
- Distinguir polling de interrupção (e justificar quando usar cada um)
- Reconhecer no `/proc/interrupts` quais dispositivos geram mais IRQs

---

## Passo a Passo da Prática

### Atividade 4.1 — Mapeando o barramento PCIe (7 min)

#### No Linux

```bash
# Listagem geral de dispositivos PCI
lspci

# Encontre o ID do controlador NVMe
lspci | grep -i nvme

# Detalhes da geração e largura do link (substitua o ID encontrado)
sudo lspci -vv -s <ID_NVMe> | grep -E "LnkCap|LnkSta"

# Mesma coisa para a GPU
lspci | grep -i vga
sudo lspci -vv -s <ID_GPU> | grep -E "LnkCap|LnkSta"
```

> **Como ler:**
> - `LnkCap` = capacidade máxima do dispositivo
> - `LnkSta` = o que está realmente sendo usado (pode ser menor!)
> - Exemplo: `Speed 8GT/s, Width x4` = PCIe 3.0 x4 ≈ ~3.94 GB/s teóricos

#### No Windows

1. Abra o **HWiNFO64**.
2. Expanda **Bus → PCI Express**.
3. Para cada dispositivo (especialmente NVMe e GPU), anote:
   - PCIe Generation (ex: 3.0, 4.0, 5.0)
   - Link Width (x1, x4, x8, x16)
   - Current Speed

#### Tabela de larguras teóricas para preencher

| Padrão PCIe | Banda por lane | x4 | x16 |
|-------------|----------------|------|------|
| PCIe 3.0 | ~0.985 GB/s | ~3.94 GB/s | ~15.8 GB/s |
| PCIe 4.0 | ~1.97 GB/s | ~7.88 GB/s | ~31.5 GB/s |
| PCIe 5.0 | ~3.94 GB/s | ~15.75 GB/s | ~63 GB/s |

### Atividade 4.2 — Interrupções em tempo real (8 min)

#### No Linux

Em um terminal, rode:

```bash
# Atualiza a cada 1 segundo
watch -n 1 'cat /proc/interrupts | head -25'
```

Em outro terminal, gere atividade nos periféricos. Os alunos observam **quais linhas de IRQ aumentam** com cada ação:

```bash
# 1. Gere tráfego de rede — observe a IRQ da placa de rede subindo
ping -c 100 8.8.8.8

# 2. Mexa o mouse — observe a IRQ do USB
# 3. Digite no teclado — observe a IRQ correspondente
# 4. Copie um arquivo grande — observe a IRQ do controlador de armazenamento
dd if=/dev/zero of=/tmp/teste.bin bs=1M count=500
rm /tmp/teste.bin
```

#### No Windows — LatencyMon

1. Abra o **LatencyMon**.
2. Clique em **Play (▶)** e deixe rodando 60 segundos.
3. Durante esse tempo, mexa o mouse, digite, abra o navegador.
4. Pare e veja:
   - **CPUs**: latência por núcleo
   - **Drivers**: ranking dos drivers que mais geram ISRs (Interrupt Service Routines) e DPCs (Deferred Procedure Calls)

> Drivers com latência alta nos DPCs são os "vilões" clássicos de stutter em aplicações em tempo real (áudio profissional, jogos competitivos).

### Atividade 4.3 — Comparação RAM vs PCIe (opcional, mas instigante)

Calculem juntos:

```
RAM DDR4-3200 dual-channel = 2 * 25.6 GB/s = 51.2 GB/s
PCIe 4.0 x16              = 31.5 GB/s
PCIe 4.0 x4 (NVMe)        = 7.88 GB/s
```

> **Pergunta para discussão:** quando você joga um jogo com texturas pesadas, o gargalo está na largura de banda da **VRAM da GPU**, do **PCIe** (pra mover texturas RAM→GPU), ou do **SSD** (pra ler do disco)? A resposta varia conforme o cenário — e essa discussão é a essência da otimização de performance.

---

## Perguntas Reflexivas

1. Calcule a largura de banda teórica de PCIe 4.0 x16. Compare com a largura de banda da sua RAM (medida no Bloco 2). **Onde fica o gargalo numa GPU dedicada moderna?**
2. Por que existem **interrupções**? Qual seria a alternativa, e por que ela é pior em quase todos os casos? *(Esperado: polling — desperdiça CPU.)*
3. Em que cenário **polling é melhor que interrupção**? *(Dica: pense em redes de altíssima taxa — DPDK, kernel bypass.)*
4. Quando você digita uma tecla, descreva a cadeia: **tecla pressionada → caractere aparece na tela**. Onde estão as interrupções, onde estão os barramentos, onde está o SO?
5. Seu SSD NVMe tem capacidade nominal de leitura de 7000 MB/s. Você verifica `lspci` e descobre que ele está em **PCIe 3.0 x4** (na placa-mãe antiga). Qual seria a leitura máxima real possível? Por que o usuário pode estar perdendo desempenho sem saber?
6. **DMA** (Direct Memory Access). O que é e por que reduz a carga sobre a CPU? Como ele se relaciona com as interrupções?

---

## Conexão com o próximo bloco

> *"Vocês têm agora todas as peças: pipeline, cache, RAM, swap, PCIe, interrupções. No próximo (e último) bloco, vocês vão escolher um workload real, medi-lo de ponta a ponta, e identificar o **gargalo dominante** da sua máquina. Voltamos à pergunta inicial: por que duas máquinas similares têm desempenhos diferentes?"*

---

[← Aula 3](../aula3-ram-armazenamento-memoria-virtual/README.md) | [Índice](../../README.md) | **Aula 4** | [Aula 5 →](../aula5-sintese-benchmark-integrado/README.md)
