# Aula 3 — RAM, Armazenamento e Memória Virtual

Duração: 20 min. Foco em RAM, armazenamento, memória virtual, paginação e swap.

## Navegação

- [Voltar ao índice do laboratório](../../README.md)
- [Aula anterior: Hierarquia de Memória e Caches](../aula2-hierarquia-memoria-caches/README.md)
- [Próxima aula: Barramentos, I/O e Interrupções](../aula4-barramentos-io-interrupcoes/README.md)

## Ponte com a teoria

Este bloco integra RAM, armazenamento e memória virtual no mesmo experimento.
A meta é observar, com dados, como o sistema operacional cria a abstração de
memória virtual e qual é o custo quando ocorre paginação para disco.

## Descrição

Este bloco separa com clareza memória (RAM) e armazenamento (disco), mostrando como a memória virtual conecta os dois. Você vai medir a diferença de desempenho e observar paginação em tempo real.

## Objetivos de Aprendizagem

Ao final deste bloco, o aluno deve ser capaz de:

- Comparar largura de banda de RAM, NVMe, SATA SSD e HDD em **ordens de grandeza**
- Distinguir Working Set, Private Bytes e Virtual Size de um processo
- Explicar o que é uma page fault e diferenciar minor de major
- Justificar configurações como `vm.swappiness` em servidores

---

## Passo a Passo da Prática

### Atividade 3.1 — Medindo o armazenamento (8 min)

#### No Windows — CrystalDiskMark

1. Abra o **CrystalDiskMark**.
2. Selecione: **5 passes**, **1 GiB**, drive do sistema.
3. Clique em "All" para rodar todos os testes.
4. Anote os valores das linhas:
   - **SEQ1M Q8T1** → leitura/escrita sequencial
   - **RND4K Q1T1** → leitura/escrita aleatória de blocos pequenos (este é o que mais sente o tipo de mídia)

#### No Linux — fio

```bash
# Leitura sequencial
fio --name=seqread --rw=read --bs=1M --size=1G --numjobs=1 \
    --runtime=30 --filename=/tmp/fio-test --direct=1 --group_reporting

# Leitura aleatória 4K
fio --name=randread --rw=randread --bs=4k --size=1G --numjobs=1 \
    --runtime=30 --filename=/tmp/fio-test --direct=1 --group_reporting

# Limpeza
rm /tmp/fio-test
```

> **Cuidado:** o `fio` com `--direct=1` ignora o cache do SO. Sem essa flag, vocês mediriam o cache de página, não o disco real.

### Atividade 3.2 — Comparação visual com a RAM (2 min)

Pegue a largura de banda da RAM medida no **Bloco 2** e compare com os números deste bloco. A tabela abaixo revela a hierarquia completa:

| Componente               | Largura de banda medida | Latência típica |
| ------------------------ | ----------------------- | --------------- |
| Cache L1                 | (do Bloco 2)            | ~1 ns           |
| Cache L3                 | (do Bloco 2)            | ~10-15 ns       |
| RAM                      | (do Bloco 2)            | ~70-100 ns      |
| SSD NVMe (SEQ1M)         |                         | ~50-200 µs      |
| SSD NVMe (RND4K)         |                         | ~50-200 µs      |
| SSD SATA (se disponível) |                         | ~80-300 µs      |
| HDD (se disponível)      |                         | ~5-15 ms        |

> Observação: destaque as ordens de grandeza na sua análise. Cache opera em **nanossegundos**, SSD em **microssegundos** e HDD em **milissegundos**.

### Atividade 3.3 — Memória Virtual em tempo real (10 min)

#### No Windows

1. Abra o **RAMMap** (Sysinternals). Mostre as categorias na aba "Use Counts":
   - **Active** (em uso ativo)
   - **Standby** (cache, pode ser realocado)
   - **Modified** (precisa ser escrito antes de liberar)
   - **Free**

2. Abra o **Process Explorer**. Selecione um processo grande (ex: navegador). Adicione as colunas:
   - **Working Set** — o que está fisicamente em RAM
   - **Private Bytes** — alocação real do processo
   - **Virtual Size** — espaço de endereçamento total

3. Note como **Virtual Size pode ser muito maior que a RAM física**. Isso é a abstração da memória virtual em ação.

#### No Linux

```bash
# Visão geral da memória física
cat /proc/meminfo | head -10

# Estatísticas em tempo real (si=swap-in, so=swap-out)
vmstat 1 5

# Mapa de memória virtual de um processo
PID=$(pgrep -f firefox | head -1)
cat /proc/$PID/status | grep -E "VmSize|VmRSS|VmSwap|VmPeak"

# Mapa detalhado das regiões
sudo cat /proc/$PID/maps | head -20
```

### Atividade 3.4 — Pressão de memória controlada (5 min)

> Aviso importante: este experimento pode forçar swap. Feche trabalhos importantes antes de executar. Se a máquina não tiver swap configurado, use apenas o script Python.

#### Linux — stress-ng

```bash
# Em um terminal, monitore:
watch -n 1 "free -h; echo; vmstat 1 1"

# Em outro terminal, gere pressão (consome 80% da RAM por 30s)
stress-ng --vm 2 --vm-bytes 80% --timeout 30s
```

Observe durante o teste:

- Queda do `free`
- Aumento de `swap used` (se houver swap)
- Colunas `si` e `so` do vmstat se movendo (swap-in/swap-out)

#### Cross-platform — Script Python

Use o script [`scripts/teste_memoria_virtual.py`](./scripts/teste_memoria_virtual.py):

```bash
python3 scripts/teste_memoria_virtual.py
```

Este script aloca buffers progressivamente maiores e mede o tempo de acesso, mostrando a inflexão dramática quando o sistema começa a paginar para o disco.

---

## Perguntas Reflexivas

1. Por que o **Virtual Size** de um processo pode ser **maior que a RAM física** da máquina? O que isso revela sobre o que de fato é um endereço de memória que aparece no seu programa em C?
2. Quando o sistema faz swap, a penalidade é de quantas ordens de grandeza comparada ao acesso à RAM? **Use os números medidos por você** para responder.
3. Defina **page fault**. Toda page fault é "ruim"? Distingua _minor page fault_ de _major page fault_.
4. Em servidores de banco de dados, configura-se `vm.swappiness=1`. Por quê? E em um desktop comum, por que o padrão é `60`?
5. RND4K é dramaticamente mais lento que SEQ1M, mesmo no mesmo SSD. Por quê? _(Dica: pense em latência vs throughput, e no que o controlador NAND precisa fazer.)_
6. Um colega afirma: _"comprei um SSD NVMe novo, agora meu PC vai ser mais rápido que ter mais RAM"_. Argumente, com **dados medidos hoje**, por que essa afirmação está parcialmente errada.

---

## Conexão com o próximo bloco

> No próximo bloco, você vai mapear o PCIe da máquina, estimar a banda teórica do link e observar interrupções geradas por dispositivos de I/O.

## Scripts deste bloco

- [`scripts/teste_memoria_virtual.py`](./scripts/teste_memoria_virtual.py) — visualiza a inflexão entre RAM e swap
