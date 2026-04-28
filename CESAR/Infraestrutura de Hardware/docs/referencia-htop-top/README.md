# Guia de Referência — Lendo `htop` e `top` sem se Perder
<!-- markdownlint-disable MD013 -->

> **Para quê este guia?**
> Durante o laboratório, vocês vão olhar para `htop` e `top` o tempo todo — para ver carga das CPUs, identificar processos pesados, observar troca de páginas, etc. Mas as colunas têm muitas siglas misteriosas. Este guia é uma **consulta rápida**: leia quando bater dúvida no que cada coluna significa.

## Navegação

- [Voltar ao índice do laboratório](../../README.md)
- [Ponte teoria-prática](../teoria-pratica-bridge/README.md)
- [Aula 1: Anatomia da CPU, Pipeline e Multicore](../aula1-anatomia-cpu-pipeline-multicore/README.md)

---

## Sumário

1. [Como abrir cada ferramenta](#1-como-abrir-cada-ferramenta)
2. [Como o `htop` aparece na tela](#2-como-o-htop-aparece-na-tela)
3. [Identificação do processo](#3-identificação-do-processo) — `PID`, `USER`, `Command`
4. [Prioridade e escalonamento](#4-prioridade-e-escalonamento) — `PRI`, `NI`
5. [Memória do processo](#5-memória-do-processo) — `VIRT`, `RES`, `SHR`, `MEM%`
6. [Estado e CPU](#6-estado-e-cpu) — `S`, `CPU%`, `TIME+`
7. [Estados possíveis (a coluna `S`)](#7-estados-possíveis-a-coluna-s)
8. [Cabeçalho do htop](#8-cabeçalho-do-htop) — barras de CPU, RAM, Swap, Load Average
9. [Comandos interativos úteis](#9-comandos-interativos-úteis)
10. [Diferenças entre `top` e `htop`](#10-diferenças-entre-top-e-htop)
11. [Cheat sheet de uma página](#11-cheat-sheet-de-uma-página)

---

## 1. Como abrir cada ferramenta

```bash
# top (vem instalado em qualquer Linux)
top

# htop (mais bonito e interativo)
htop

# Se não estiver instalado:
sudo apt install htop
```

Para sair: aperte `q`.

---

## 2. Como o `htop` aparece na tela

Quando vocês rodam `htop`, a tela tem duas partes:

```text
┌─────────────────────────────────────────────────────────────────┐
│   1 [||||||||||           23.4%]   Tasks: 247, 412 thr; 2 run   │  ← CABEÇALHO
│   2 [||||                  10.1%]   Load average: 0.85 0.62 0.48│
│ Mem [||||||||||||      6.2G/16.0G]  Uptime: 03:42:15            │
│ Swp [                       0K/2G]                              │
├─────────────────────────────────────────────────────────────────┤
│  PID USER  PRI  NI  VIRT   RES   SHR S  CPU%  MEM%   TIME+ Command│  ← CABEÇALHO DA TABELA
│ 1234 roni   20   0  4.8G  512M   45M S   2.3   3.2  0:12.45 firefox│
│ 5678 roni   20   0  280M   85M   12M R  98.0   0.5  0:34.10 python │  ← LINHAS DE PROCESSOS
│  ...                                                              │
└─────────────────────────────────────────────────────────────────┘
```

Vou explicar **cada coluna** abaixo.

---

## 3. Identificação do Processo

### `PID` — Process ID

Número único que o sistema operacional atribui a cada processo. É a "identidade" do processo enquanto ele está vivo.

> **Para que serve saber:** se quiserem matar um processo travado, vocês usam o PID: `kill 5678`. Quando um processo termina e outro novo nasce, o PID pode (e geralmente vai) mudar.

### `USER`

Quem é o dono do processo. Normalmente seu próprio usuário, ou `root` para processos do sistema.

### `Command` (ou `COMMAND`)

O nome do programa em execução, com os argumentos que ele recebeu. Exemplos:

- `firefox` — o navegador
- `python3 meu_script.py` — script Python rodando
- `/usr/bin/gcc -O2 main.c` — compilador GCC

> **Dica:** no `htop`, aperte `F5` para ver os processos como uma **árvore**, mostrando quem é filho de quem. Útil para ver, por exemplo, que o navegador tem dezenas de processos filhos (cada aba é um deles).

---

## 4. Prioridade e Escalonamento

Aqui é onde mora o "quem fura a fila da CPU". O kernel decide qual processo roda em qual momento, e essas duas colunas influenciam essa decisão.

### `PRI` — Priority (Prioridade interna do kernel)

Número que o **kernel** usa para decidir a ordem de execução. Quanto **menor** o número, **maior** a prioridade.

- Valores típicos: `0` a `139`
- Processos de usuário comuns: aparecem com `PRI = 20`
- Processos de tempo real: podem ter PRI **negativa** (mais altíssima prioridade)

> Não se preocupem em mexer diretamente em PRI. Vocês influenciam ela indiretamente, através do `NI`.

### `NI` — Nice value (o valor "amabilidade")

É o valor que o **usuário** pode ajustar. A ideia é: quanto mais "nice" (educado) um processo é, mais ele cede a vez para outros.

- Faixa: `-20` (super egoísta, prioridade máxima) até `+19` (super gentil, prioridade mínima)
- Padrão: `0` (neutro)

```bash
# Rodar um comando com prioridade baixa (não atrapalha o sistema)
nice -n 19 ./meu_script_pesado.sh

# Mudar a prioridade de um processo já rodando (precisa do PID)
renice -n 10 -p 5678
```

> **Pegadinha pedagógica:** a relação geralmente é `PRI = 20 + NI`. Se vocês colocam `NI = -5`, o `PRI` cai para 15 — ou seja, **o processo passa na frente dos demais**.
> **Exemplo prático no laboratório:** se quiserem rodar um benchmark sem interferência de outros processos, usem `nice -n -10 ./benchmark` (precisa de `sudo`). O contrário também vale: jogos pesados em background podem rodar com `nice 19` para não engasgar a aula.

---

## 5. Memória do Processo

Aqui é a parte que **conecta direto com o Bloco 3 da nossa aula** (Memória Virtual). Cada coluna mostra um pedaço diferente do quebra-cabeça.

### `VIRT` — Virtual Size (Tamanho Virtual)

Todo o **espaço de endereçamento** que o processo *acha* que tem. Inclui:

- Código do programa
- Bibliotecas compartilhadas
- Memória alocada (mesmo que ainda não usada de verdade)
- Mapeamentos de arquivos
- Espaço para a pilha (stack) de cada thread

> **Pode ser ENORME.** Um navegador moderno tem `VIRT` de vários gigabytes — e isso é normal! É a memória virtual fazendo o que ela faz: **prometer** muito espaço. Só uma fração disso está realmente em RAM.

### `RES` — Resident Size (Tamanho Residente em RAM)

Quanto da memória do processo está **fisicamente em RAM neste momento**. É o número que importa quando alguém pergunta "esse programa tá comendo muita memória?".

```text
VIRT = "o que prometi alocar"     (mundo virtual, pode ser grande)
RES  = "o que tá em RAM agora"    (mundo físico, é o que dói)
```

### `SHR` — Shared Memory (Memória Compartilhada)

Parte do `RES` que está **compartilhada** com outros processos. Tipicamente:

- Bibliotecas como `libc`, `libssl` (vários programas usam ao mesmo tempo)
- Páginas mapeadas de arquivos abertos por múltiplos processos

> **Por que importa:** quando o sistema mostra que o navegador usa 800 MB de RAM, parte disso (o `SHR`) pode estar sendo compartilhada com outros programas. A "memória real exclusiva" do processo é mais ou menos `RES − SHR`.

### `MEM%`

Percentual da RAM total da máquina que o processo está usando (= `RES` ÷ `RAM total` × 100).

### Resumo visual

```text
       ┌─────────────────────────────────────────────┐
 VIRT  │ Tudo que o processo "promete" usar          │
       │ (pode ser maior que a RAM física!)          │
       └─────────────────────────────────────────────┘
              ↓ apenas parte está REALMENTE em RAM
       ┌──────────────────┐
 RES   │ Em RAM agora     │
       └──────────────────┘
              ↓ parte é compartilhada com outros processos
       ┌─────────┐
 SHR   │ Compart.│
       └─────────┘
```

---

## 6. Estado e CPU

### `S` — State (Estado do processo)

Letra única indicando o que o processo está fazendo agora mesmo. Os principais estados estão na próxima seção.

### `CPU%`

Percentual de uso de CPU que o processo está consumindo.

> **Cuidado:** este percentual é **por núcleo**! Em uma máquina com 8 threads, um processo single-thread no máximo chega a `100%`. Um processo bem paralelizado pode aparecer com `780%` (ocupando quase 8 núcleos completos). Não é bug.

### `TIME+`

Tempo **acumulado** de CPU que o processo consumiu desde que nasceu. Formato: `MM:SS.cc` (minutos, segundos, centésimos).

> **Útil para identificar processos zumbis ou em loop:** se um processo está com `CPU%` baixo mas o `TIME+` cresce rapidinho, ele pode estar em loop sem você perceber.

---

## 7. Estados Possíveis (a coluna `S`)

Essa é a parte que mais confunde — vou destrinchar todos:

| Letra | Nome                         | O que significa                                                                       |
| ----- | ---------------------------- | ------------------------------------------------------------------------------------- |
| **R** | Running                      | Está rodando agora ou pronto para rodar (na fila do scheduler)                        |
| **S** | Sleeping                     | Esperando algo acontecer (ex: input do usuário, dado da rede). É o estado mais comum. |
| **D** | Disk sleep (uninterruptible) | Esperando o disco/dispositivo. **Não pode ser interrompido nem com `kill -9`.**       |
| **T** | Stopped                      | Parado (você pausou com `Ctrl+Z` ou enviou SIGSTOP)                                   |
| **t** | Tracing stop                 | Pausado por um debugger (gdb, strace)                                                 |
| **Z** | Zombie                       | Já terminou, mas o processo pai ainda não leu o status de saída                       |
| **X** | Dead                         | Morrendo (raríssimo de ver)                                                           |
| **I** | Idle                         | Thread do kernel ociosa (estado novo, em kernels recentes)                            |

### Os mais importantes para a vida real

#### `R` (Running) e `S` (Sleeping) — o normal

Em um sistema saudável, **a maioria dos processos está em `S`**. Um navegador aberto, um editor de texto, o próprio terminal... estão dormindo, esperando algo. Eles só acordam quando há trabalho.

#### `D` (Disk sleep) — atenção

Quando aparece muito processo em `D`, sinal de **gargalo de I/O**. O processo está tão preso esperando o disco que nem o `kill` resolve. Isso conecta direto com o **Bloco 3** (memória virtual + swap): se o sistema está paginando demais, vários processos ficam em `D`.

#### `Z` (Zombie) — não é bug do filme

Processo zumbi não consome CPU nem RAM, mas ocupa uma entrada na tabela de processos. Surge quando o pai esqueceu de chamar `wait()` para coletar o status do filho que terminou. Em geral, somem quando o pai morre.

> **Curiosidade:** o nome técnico é "zombie" mesmo, e a coluna mostra `Z`. Não é gíria.

---

## 8. Cabeçalho do htop

A parte de cima da tela tem informações **agregadas do sistema todo** — diferentes das colunas que falam de processos individuais.

### Barras de CPU

Uma barra por núcleo lógico. Cada barra mostra a **divisão da carga**:

```text
1 [|||||||||||||||||                45.2%]
   ↑                ↑               ↑
   azul/verde       cinza           percentual total
   (user)           (idle)
```

Cores típicas:

- **Verde:** tempo gasto rodando código de usuário (`user`)
- **Azul:** tempo de processos com `nice` baixo
- **Vermelho:** tempo gasto em modo kernel (`system`)
- **Laranja:** I/O wait (esperando disco)
- **Roxo:** soft IRQs

> No `top`, a mesma informação aparece como uma linha de texto: `%Cpu(s): 25.0 us, 5.0 sy, 0.0 ni, 70.0 id, 0.0 wa`. Significa: 25% user, 5% system, 70% idle, 0% I/O wait.

### `Mem` — barra de memória

```text
Mem [|||||||||||||||||||||      6.2G/16.0G]
     ↑                          ↑      ↑
     usado                       usado / total
```

- Verde: memória ocupada por processos
- Azul: buffers (metadados de arquivos)
- Amarelo: cache de páginas (conteúdo de arquivos lidos recentemente)

> **A parada do "Linux comeu minha RAM":** o cache amarelo **não é desperdício**. O Linux usa RAM ociosa como cache para acelerar leitura de arquivos. Quando um processo precisa, o cache é liberado automaticamente. Sistema com RAM "cheia" de cache é sistema **bem usado**, não cheio.

### `Swp` — barra de swap

Quanto do swap está sendo usado. **Se essa barra começar a encher durante o lab, vocês estão vendo paginação acontecer ao vivo** (Bloco 3!).

### `Load Average`

Três números — média de carga em **1, 5 e 15 minutos**:

```text
Load average: 0.85 0.62 0.48
              ↑    ↑    ↑
              1min 5min 15min
```

Como interpretar:

- Em uma máquina com **N núcleos lógicos**, load = N significa "100% de carga".
- Load de `2.0` em uma máquina de 8 núcleos = 25% de carga (folgada).
- Load de `8.0` em uma máquina de 8 núcleos = saturada.
- Load de `16.0` em uma máquina de 8 núcleos = sobrecarregada (processos esperando).

> Os três números mostram **tendência**: se o de 1 min é alto e o de 15 min é baixo, a máquina **acabou de sobrecarregar**. O contrário indica que **está aliviando**.

### `Tasks`

```text
Tasks: 247, 412 thr; 2 running
       ↑    ↑         ↑
       processos   threads   rodando agora
```

---

## 9. Comandos Interativos Úteis

Enquanto o `htop` está aberto, vocês podem apertar:

| Tecla        | O que faz                                           |
| ------------ | --------------------------------------------------- |
| `F1`         | Ajuda                                               |
| `F2`         | Configurações (cores, colunas mostradas)            |
| `F3` ou `/`  | Buscar processo pelo nome                           |
| `F4`         | Filtrar (mostra só processos que batem com o termo) |
| `F5`         | Vista em árvore (mostra hierarquia pai → filho)     |
| `F6`         | Mudar critério de ordenação                         |
| `F7` / `F8`  | Diminuir / aumentar `nice` do processo selecionado  |
| `F9`         | Matar o processo selecionado (vai pedir o sinal)    |
| `F10` ou `q` | Sair                                                |
| `H`          | Mostrar/ocultar threads de usuário                  |
| `K`          | Mostrar/ocultar threads do kernel                   |
| `Espaço`     | Marcar processo (para ações em lote)                |
| `t`          | Vista em árvore (alternativa ao F5)                 |

### Comandos equivalentes para o `top`

| Tecla | O que faz                                 |
| ----- | ----------------------------------------- |
| `M`   | Ordenar por uso de memória                |
| `P`   | Ordenar por uso de CPU                    |
| `T`   | Ordenar por tempo acumulado               |
| `k`   | Matar processo (pede o PID)               |
| `r`   | Mudar `nice` (renice)                     |
| `1`   | Mostrar uma barra por CPU em vez de média |
| `q`   | Sair                                      |

---

## 10. Diferenças entre `top` e `htop`

| Característica         | `top`    | `htop`           |
| ---------------------- | -------- | ---------------- |
| Sempre instalado?      | Sim      | Precisa instalar |
| Cores e barras visuais |          |                  |
| Mouse funciona         |          |                  |
| Vista em árvore        | Limitada | Boa              |
| Scroll horizontal      |          |                  |
| Configurável           | Pouco    | Bastante         |

> **Use `top`** quando estiver em servidor enxuto sem `htop` instalado, ou em scripts (`top -b -n 1`).
> **Use `htop`** sempre que possível — é mais legível, especialmente para alunos.

---

## 11. Cheat Sheet de Uma Página

```text
╔══════════════════════════════════════════════════════════════════╗
║  IDENTIFICAÇÃO                                                   ║
║   PID    = ID único do processo                                  ║
║   USER   = dono                                                  ║
║   CMD    = nome do programa                                      ║
║                                                                  ║
║  PRIORIDADE                                                      ║
║   PRI    = prioridade interna (menor = mais prioridade)          ║
║   NI     = nice (-20 = egoísta, +19 = gentil; padrão = 0)        ║
║                                                                  ║
║  MEMÓRIA                                                         ║
║   VIRT   = espaço virtual prometido (pode ser grande, normal)    ║
║   RES    = realmente em RAM agora ← o que importa                ║
║   SHR    = parte do RES compartilhada com outros processos       ║
║   MEM%   = % da RAM total                                        ║
║                                                                  ║
║  CPU                                                             ║
║   CPU%   = uso de CPU (pode passar de 100% se multi-thread)      ║
║   TIME+  = tempo total de CPU acumulado                          ║
║                                                                  ║
║  ESTADO (S)                                                      ║
║   R = running         S = sleeping (normal)                      ║
║   D = disk wait      T = stopped (Ctrl+Z)                       ║
║   Z = zombie          I = idle kernel thread                     ║
║                                                                  ║
║  CABEÇALHO                                                       ║
║   Verde   = user            Vermelho  = system                   ║
║   Azul    = nice            Laranja   = I/O wait                 ║
║   Amarelo = cache (não é "RAM cheia"!)                           ║
║                                                                  ║
║  LOAD AVG  = carga em 1min, 5min, 15min                          ║
║   compare com nº de núcleos: load=N significa 100% saturado      ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## Quando consultar este guia durante o laboratório?

- **Bloco 1 (CPU/Pipeline):** olhem as barras de CPU enquanto rodam o Cinebench. O teste single-core acende **uma barra**; o multi-core acende **todas**. Vejam o `CPU%` do processo do benchmark.
- **Bloco 3 (RAM/Swap):** observem a barra `Mem` e `Swp` enquanto rodam o `stress-ng`. Vejam o `VIRT` vs `RES` do processo.
- **Bloco 4 (Interrupções):** comparem a divisão `user` vs `system` na barra de CPU. Tarefas com muita I/O têm `system` (vermelho) maior.
- **Bloco 5 (Síntese):** usem `htop` enquanto roda o workload completo. Identifiquem visualmente onde está o gargalo.

---

> **Referências para se aprofundar (opcional):**
>
> - [`docs/teoria-pratica-bridge/`](../teoria-pratica-bridge/README.md) — mapa entre os 14 tópicos da ementa e os experimentos do laboratório.
> - `man htop` e `man top` — documentação oficial.
> - "Linux Performance" de Brendan Gregg (livro).
> - Site brendangregg.com — gráficos e ferramentas de análise de performance.
