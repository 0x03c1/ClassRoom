# Kit de Análise de Performance de Algoritmos

Duração sugerida: uso complementar ao laboratório. Foco em complexidade
empírica, profiling e comparação entre linguagens.

## Navegação

- [Voltar ao índice do laboratório](../../README.md)
- [Ponte teoria-prática](../teoria-pratica-bridge/README.md)
- [Modelo de relatório](../relatorio-template/relatorio.md)

## Para que serve este material?

Este material ajuda a responder, com **medições reais e gráficos**,
perguntas como:

- Esse algoritmo é O(n), O(n log n) ou O(n²)?
- Minha implementação em Python é quantas vezes mais lenta que em C ou Java?
- O mesmo algoritmo roda mais rápido em qual máquina, e por quê?
- Onde o programa gasta tempo de verdade e qual função é o gargalo?

O kit reúne **três ferramentas complementares**, cada uma para um tipo de
análise, com scripts prontos para integrar com o resto do laboratório.

## Visão geral

```text
analise-performance-algoritmos/
├── README.md
├── scripts/
│   ├── big_o_visualizador.py
│   ├── comparador_stacks.py
│   └── flame_graph_helper.sh
└── exemplos/
    ├── ordenacao.py
    ├── ordenacao.c
    └── Ordenacao.java
```

## Acesso rápido

### Scripts para efetuar as medições

- [big_o_visualizador.py](./scripts/big_o_visualizador.py) — medir curva de complexidade empírica
- [flame_graph_helper.sh](./scripts/flame_graph_helper.sh) — apoiar profiling com `perf` e flame graph
- [comparador_stacks.py](./scripts/comparador_stacks.py) — comparar linguagens e máquinas

### Exemplos para testar

- [ordenacao.py](./exemplos/ordenacao.py) — exemplo em Python
- [ordenacao.c](./exemplos/ordenacao.c) — exemplo em C
- [Ordenacao.java](./exemplos/Ordenacao.java) — exemplo em Java

## Casos de uso

| Caso                           | Ferramenta                        | Pergunta que responde                                       | Saída                                  |
| ------------------------------ | --------------------------------- | ----------------------------------------------------------- | -------------------------------------- |
| 1. Curva de complexidade       | `big_O` + script próprio          | Qual é a complexidade empírica do algoritmo?                | Gráfico log-log com curva ajustada     |
| 2. Hot spots e profiling       | `perf`, `hyperfine` e Flame Graph | Onde o programa gasta tempo e como esse tempo se distribui? | SVG interativo + relatório estatístico |
| 3. Comparação entre linguagens | Framework próprio em Python       | Como C, Python e Java se comparam na mesma máquina?         | CSV + gráfico comparativo              |

Os três casos se complementam: o Caso 1 valida a teoria, o Caso 2 revela o
gargalo e o Caso 3 compara stacks e máquinas.

---

## Caso 1 — Curva de Complexidade Empírica

Este caso responde à pergunta clássica de Algoritmos e Estrutura de Dados:
_"prove que seu algoritmo é O(n log n)"_. Aqui a resposta vem pelo lado
empírico, medindo tempos com tamanhos crescentes de entrada.

### Ferramenta do Caso 1

A biblioteca `big_O` recebe uma função e uma série de tamanhos de entrada,
executa o algoritmo, mede os tempos e infere a classe de complexidade que
melhor se ajusta aos dados observados.

### Instalação do Caso 1

```bash
pip install big_O numpy matplotlib --break-system-packages
```

### Uso básico

```bash
python3 scripts/big_o_visualizador.py
```

Script de medição: [big_o_visualizador.py](./scripts/big_o_visualizador.py)

Exemplos de apoio:

- [ordenacao.py](./exemplos/ordenacao.py)
- [ordenacao.c](./exemplos/ordenacao.c)
- [Ordenacao.java](./exemplos/Ordenacao.java)

O script já vem com algoritmos prontos para comparação:

- **Soma linear** — esperado: O(n)
- **Busca binária** — esperado: O(log n)
- **Bubble sort** — esperado: O(n²)
- **Sort embutido (Timsort)** — esperado: O(n log n)

### Saída esperada do Caso 1

O script gera:

1. Uma tabela no terminal com a complexidade inferida.
2. O gráfico `complexidade_empirica.png` com as curvas em escala log-log.

```text
Algoritmo              Complexidade Inferida    Bate com a teoria?
─────────────────────────────────────────────────────────────────
Soma linear           Linear: O(n)              sim
Busca binária         Logarítmica: O(log n)     sim
Bubble sort           Quadrática: O(n²)         sim
Timsort               Linearítmica: O(n log n)  sim
```

### Como adaptar ao seu algoritmo

Edite `scripts/big_o_visualizador.py` adicionando ao dicionário `ALGORITMOS`:

```python
ALGORITMOS = {
    "Meu algoritmo": (minha_funcao, gerador_de_entrada),
}
```

Pergunta sugerida para os alunos: a curva empírica bate com a complexidade
teórica discutida em sala?

---

## Caso 2 — Hot Spots e Profiling Profundo

Este caso é a ponte direta com a síntese final do laboratório e com os
conceitos de pipeline e cache. O flame graph mostra **em qual parte do código**
o tempo está sendo gasto, e o `perf stat` ajuda a explicar o motivo.

### Ferramentas do Caso 2

| Ferramenta                    | Uso principal                        |
| ----------------------------- | ------------------------------------ |
| `hyperfine`                   | Benchmark estatístico de comandos    |
| `perf record` + `perf script` | Coleta de stacks durante a execução  |
| `flamegraph.pl`               | Geração de flame graph em SVG        |
| `py-spy`                      | Profiling por amostragem para Python |
| `async-profiler`              | Profiling para Java/JVM              |

### Instalação do Caso 2

```bash
sudo apt install -y linux-tools-common linux-tools-generic hyperfine

# (Se "linux-tools-$(uname -r)" não vier junto, instale também)
sudo apt install -y "linux-tools-$(uname -r)" || true

git clone --depth=1 https://github.com/brendangregg/FlameGraph.git ~/FlameGraph

# Adiciona ao PATH para sessões futuras
echo 'export PATH="$HOME/FlameGraph:$PATH"' >> ~/.bashrc

# E para ESTA sessão também (essencial — sem isso, flamegraph.pl
# ainda não estará no PATH)
export PATH="$HOME/FlameGraph:$PATH"

pip install py-spy --break-system-packages
```

> **Nota WSL2:** o pacote `linux-tools-$(uname -r)` pode não existir, pois
> o kernel é da Microsoft. Nesse caso, é necessário compilar `perf` do
> source. Veja: <https://github.com/microsoft/WSL2-Linux-Kernel>.

### Fluxo recomendado

```bash
# 1. Benchmark estatístico
hyperfine --warmup 3 \
  './versao_lenta' \
  './versao_otimizada' \
  --export-markdown comparacao.md

# 2. Profiling profundo (apenas o processo, NÃO o sistema todo)
sudo perf record -F 99 -g -- ./versao_lenta
sudo perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg

# 3. Abrir o resultado
xdg-open flame.svg
```

> **Sobre `-a` (system-wide):** se quiser ver tudo que está rodando no
> sistema (útil para investigar latência de SO inteiro), adicione `-a` ao
> `perf record`. Para perfilar **só o seu programa**, omita — o flame
> graph fica muito mais legível, sem ruído de daemons e idle do kernel.

Script de apoio para medições: [flame_graph_helper.sh](./scripts/flame_graph_helper.sh)

### Para Python

```bash
py-spy record -o flame.svg -- python3 meu_script.py
```

### Para Java

```bash
java -jar minha_aplicacao.jar &
JAVA_PID=$!
./async-profiler/profiler.sh -d 30 -f flame.html $JAVA_PID
```

### Como ler um flame graph

```text
main
processa_dados
ordena | filtra | formata
comp.  | regex_match  <- gargalo mais largo
```

- **Largura** representa tempo gasto.
- **Altura** representa profundidade da pilha de chamadas.
- **Cor** é apenas um auxílio visual.

### Relacionando com `perf stat`

Após localizar o gargalo, rode:

```bash
sudo perf stat -e cycles,instructions,cache-misses,cache-references,branch-misses \
  ./meu_programa
```

Interpretação útil:

- **IPC baixo** indica pipeline engasgando.
- **Cache miss alto** indica algoritmo cache-hostil.
- **Branch miss alto** indica muitos desvios imprevisíveis.

---

## Caso 3 — Comparação Multi-Linguagem e Multi-Máquina

Este caso compara a mesma operação algorítmica em linguagens diferentes e,
quando desejado, em máquinas diferentes. É um bom fechamento entre teoria,
implementação e arquitetura.

### Ferramenta

O script `comparador_stacks.py`:

1. Recebe uma lista de comandos.
2. Executa cada comando múltiplas vezes com tamanhos crescentes.
3. Mede tempo de parede, uso de CPU e pico de memória.
4. Gera CSV e gráfico comparativo.

### Uso

Para Java, primeiro compile (gera `Ordenacao.class` no mesmo diretório):

```bash
javac exemplos/Ordenacao.java
```

Depois rode a comparação:

```bash
python3 scripts/comparador_stacks.py \
  --tamanhos 1000 5000 10000 50000 100000 \
  --repeticoes 5 \
  --comandos \
    "python3 exemplos/ordenacao.py" \
    "./exemplos/ordenacao_c" \
    "java -cp exemplos Ordenacao"
```

  Script de medição: [comparador_stacks.py](./scripts/comparador_stacks.py)

  Exemplos usados nessa comparação:

- [ordenacao.py](./exemplos/ordenacao.py)
- [ordenacao.c](./exemplos/ordenacao.c)
- [Ordenacao.java](./exemplos/Ordenacao.java)

### Saída esperada

1. `resultados.csv` com as medições brutas.
2. `comparacao_stacks.png` com o gráfico comparativo.

```text
Tamanho   Python       C            Java         C/Python   Java/Python
1000      0.012s       0.0001s      0.045s       120x       0.27x
10000     0.150s       0.001s       0.082s       150x       1.83x
100000    1.820s       0.012s       0.156s       152x       11.7x
```

### Comparação entre máquinas

Você pode rodar o mesmo `comparador_stacks.py` na máquina do laboratório
e na máquina pessoal. Isso produz uma base comparável entre hardware,
linguagem e tamanho de entrada.

Pergunta de relatório sugerida: em qual stack a diferença entre máquinas foi
mais perceptível, e por quê?

---

## Quando usar cada caso?

| Pergunta                                       | Caso recomendado       |
| ---------------------------------------------- | ---------------------- |
| Qual é a complexidade do algoritmo na prática? | Caso 1                 |
| Onde o programa está lento?                    | Caso 2                 |
| A medição está estável? Há outliers?           | Caso 2 com `hyperfine` |
| Linguagem A é mais rápida que B?               | Caso 3                 |
| Esta máquina é mais rápida que aquela?         | Caso 3                 |

---

## Integração com o restante do laboratório

| Bloco da aula                 | Aplicação deste kit                                      |
| ----------------------------- | -------------------------------------------------------- |
| Aula 1 — Pipeline / Multicore | `hyperfine` para validar speedup                         |
| Aula 2 — Cache                | `perf stat -e cache-misses` no experimento de localidade |
| Aula 3 — Memória Virtual      | Caso 3 com entradas grandes e pressão de memória         |
| Aula 4 — I/O e Interrupções   | `perf record` durante I/O pesado                         |
| Aula 5 — Síntese              | Os três casos integrados no relatório final              |

Veja também [../teoria-pratica-bridge/README.md](../teoria-pratica-bridge/README.md)
para o mapa entre a ementa e os experimentos do laboratório.

---

## Sugestão de mini-projeto integrador

Proposta para os alunos:

> Escolha um algoritmo, implemente em **Python e em C** e use:
>
> - o Caso 1 para verificar a classe de complexidade,
> - o Caso 3 para medir quantas vezes C é mais rápido que Python,
> - o Caso 2 para localizar o gargalo e explicar a diferença.
>
> Entregue um relatório curto com gráficos e conclusão fundamentada.

Esse mini-projeto conversa bem com os 14 tópicos da ementa.

---

## Referências

- [`big_O` no PyPI](https://pypi.org/project/big-O/)
- [`hyperfine` no GitHub](https://github.com/sharkdp/hyperfine)
- [Flame Graphs, de Brendan Gregg](https://www.brendangregg.com/flamegraphs.html)
- [`py-spy` no GitHub](https://github.com/benfred/py-spy)
- [`async-profiler` no GitHub](https://github.com/async-profiler/async-profiler)
- _Systems Performance — Enterprise and the Cloud_, Brendan Gregg
