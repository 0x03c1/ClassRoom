# Profiling de Software — C e Python


Este repositório reúne um roteiro hands-on para você **medir, analisar e visualizar o desempenho** de programas em C e Python usando ferramentas padrão da indústria: `gprof`, `gprof2dot`, `Graphviz`, `cProfile`, `SnakeViz` e `py-spy`.

Ao final do roteiro, você será capaz de:

- Compilar um programa em C com suporte a profiling.
- Gerar e interpretar relatórios de tempo de execução por função.
- Produzir **grafos de chamadas** e **flame graphs** para apresentar resultados.
- Aplicar a mesma metodologia em scripts Python.

---

## Sumário

1. [Estrutura do Repositório](#1-estrutura-do-repositório)
2. [Pré-requisitos](#2-pré-requisitos)
3. [Parte 1 — Profiling em C com gprof](#3-parte-1--profiling-em-c-com-gprof)
4. [Parte 2 — Visualização com gprof2dot e Graphviz](#4-parte-2--visualização-com-gprof2dot-e-graphviz)
5. [Parte 3 — Profiling em Python com cProfile](#5-parte-3--profiling-em-python-com-cprofile)
6. [Parte 4 — Visualização Interativa com SnakeViz](#6-parte-4--visualização-interativa-com-snakeviz)
7. [Parte 5 — Flame Graphs com py-spy](#7-parte-5--flame-graphs-com-py-spy)
8. [Comparação das Ferramentas](#8-comparação-das-ferramentas)
9. [Solução de Problemas](#9-solução-de-problemas)
10. [Atividade Proposta](#10-atividade-proposta)
11. [Referências](#11-referências)

---

## 1. Estrutura do Repositório

```text
.
├── README.md           # este arquivo
├── test_gprof.c        # programa de exemplo em C
├── teste.py            # programa de exemplo em Python
├── programa            # binário gerado (após compilação)
├── gmon.out            # dados do gprof (após execução)
├── profile.out         # dados do cProfile (após execução)
├── grafo.png           # grafo de chamadas em C
└── profile.svg         # flame graph do py-spy
```

> Os arquivos `programa`, `gmon.out`, `profile.out`, `grafo.png` e `profile.svg` são **gerados durante o roteiro** — não precisam estar no repositório inicial.

---

## 2. Pré-requisitos

### Ambiente recomendado

- **Linux** (Ubuntu 22.04+) ou **WSL2** no Windows.
- **macOS** funciona, mas `gprof` é nativo do GNU toolchain; veja [seção 9](#9-solução-de-problemas) para alternativas.

### Pacotes do sistema

```bash
sudo apt update
sudo apt install -y build-essential graphviz python3 python3-pip
```

### Pacotes Python

```bash
pip install gprof2dot snakeviz py-spy
```

> Se o sistema bloquear a instalação global (PEP 668), use `--break-system-packages` ou um `venv`.

---

## 3. Parte 1 — Profiling em C com gprof

### 3.1. Código de exemplo

Arquivo `test_gprof.c`:

```c
#include <stdio.h>

void new_func1(void);

void new_func1(void) {
    printf("\n Inside new_func1 \n");
}

void func1(void) {
    printf("\n Inside func1 \n");
    for (unsigned long i = 0; i < 0xffffffff; i++);
    new_func1();
}

static void func2(void) {
    printf("\n Inside func2 \n");
    for (unsigned long i = 0; i < 0xffffffaa; i++);
}

int main(void) {
    printf("\n Inside main()\n");
    for (unsigned long i = 0; i < 0xffffff; i++);
    func1();
    func2();
    return 0;
}
```

### 3.2. Compilação

```bash
gcc -Wall -pg test_gprof.c -o programa
```

| Flag | Função |
|---|---|
| `-Wall` | Ativa todos os warnings importantes do compilador. |
| `-pg` | **Instrumenta o binário** para coletar dados de profiling. |
| `-o programa` | Define o nome do executável de saída. |

> Sem a flag `-pg`, o arquivo `gmon.out` **não será gerado**.

### 3.3. Execução

```bash
./programa
```

Ao terminar, será criado o arquivo `gmon.out` no diretório atual, contendo os dados brutos coletados pelo profiler.

### 3.4. Gerando o relatório textual

```bash
gprof programa gmon.out > relatorio.txt
```

O relatório tem duas partes principais:

- **Flat profile** — tempo gasto em cada função, ordenado do maior para o menor.
- **Call graph** — quem chamou quem, e quanto tempo cada chamada custou.

Trecho típico do flat profile:

```text
  %   cumulative   self              self     total
 time   seconds   seconds    calls   s/call   s/call  name
 56.21      6.32     6.32        1     6.32     6.32  func1
 43.79     11.24     4.92        1     4.92     4.92  func2
  0.00     11.24     0.00        1     0.00     6.32  new_func1
```

**Como ler:** `func1` consumiu **56,21% do tempo total** sozinha (`self seconds`).

---

## 4. Parte 2 — Visualização com gprof2dot e Graphviz

Relatórios em texto são úteis, mas grafos comunicam melhor. O pipeline é:

```text
gprof  →  gprof2dot  →  dot (Graphviz)  →  PNG/SVG
```

### 4.1. Gerar PNG

```bash
gprof programa gmon.out | gprof2dot -f prof | dot -Tpng -o grafo.png
```

### 4.2. Gerar SVG (recomendado para apresentações)

```bash
gprof programa gmon.out | gprof2dot -f prof | dot -Tsvg -o grafo.svg
```

> SVG é vetorial — escala sem perder qualidade, ideal para slides e relatórios.

### 4.3. Visualizar

**WSL:**

```bash
explorer.exe .
```

**Linux nativo:**

```bash
xdg-open grafo.png
```

**macOS:**

```bash
open grafo.png
```

---

## 5. Parte 3 — Profiling em Python com cProfile

`cProfile` é o profiler oficial da biblioteca padrão do Python — nenhuma instalação adicional necessária.

### 5.1. Código de exemplo

Arquivo `teste.py`:

```python
def func3():
    for _ in range(1_000_000):
        pass


def func2():
    func3()


def func1():
    func2()
    func3()


def main():
    func1()


if __name__ == "__main__":
    main()
```

### 5.2. Executar com profiling

```bash
python3 -m cProfile -o profile.out teste.py
```

### 5.3. Gerar grafo de chamadas

```bash
gprof2dot -f pstats profile.out | dot -Tpng -o grafo_python.png
```

---

## 6. Parte 4 — Visualização Interativa com SnakeViz

SnakeViz transforma o `profile.out` em uma página web navegável.

```bash
snakeviz profile.out
```

Isso abre automaticamente o navegador em `http://localhost:8080/`, com dois modos de visualização:

- **Icicle** — gráfico em camadas (recomendado para começar).
- **Sunburst** — gráfico radial.

> No WSL, o navegador pode não abrir sozinho. Acesse manualmente o endereço impresso no terminal.

---

## 7. Parte 5 — Flame Graphs com py-spy

`py-spy` é um **sampling profiler** — coleta amostras periódicas da stack sem modificar o código. Funciona até em processos **já em execução**.

### 7.1. Gerar flame graph

```bash
py-spy record -o profile.svg -- python3 teste.py
```

Abra `profile.svg` no navegador. **Largura = tempo gasto** em cada função.

### 7.2. Modo top (htop-like)

```bash
py-spy top -- python3 teste.py
```

---

## 8. Comparação das Ferramentas

| Ferramenta | Linguagem | Tipo | Quando usar |
|---|---|---|---|
| **gprof** | C/C++ | Instrumentado | Programas nativos compilados com `-pg`. |
| **gprof2dot** | C / Python | Conversor | Transformar relatórios em grafos. |
| **Graphviz (dot)** | Universal | Renderizador | Gerar PNG/SVG a partir do DOT. |
| **cProfile** | Python | Determinístico | Profiling padrão de scripts. |
| **SnakeViz** | Python | Visualização web | Análise interativa em sala de aula. |
| **py-spy** | Python | Sampling (amostragem) | Aplicações reais e processos em produção. |

> **Determinístico vs Sampling:** profilers determinísticos (gprof, cProfile) registram **toda** chamada de função — mais precisos, mas com overhead maior. Sampling (py-spy) tira "fotos" periódicas — overhead baixo, ideal para produção.

---

## 9. Solução de Problemas

### `undefined reference to 'funcao'`

A função foi **declarada** mas não **implementada**. Verifique se o corpo da função existe e se você está compilando o arquivo correto.

### `gmon.out` não foi gerado

Verifique se:

- O programa foi compilado com a flag `-pg`.
- O programa foi executado até o final (sem `kill`, `Ctrl+C` ou `exit` abrupto).
- Você tem permissão de escrita no diretório atual.

### `dot: command not found`

Instale o Graphviz:

```bash
sudo apt install graphviz
```

### `pip install` bloqueado (PEP 668)

Use um ambiente virtual:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install gprof2dot snakeviz py-spy
```

### gprof no macOS

O `gprof` nativo do Xcode tem limitações. Recomendado usar **Instruments** (parte do Xcode) ou rodar o roteiro em uma VM/container Linux.

---

## 10. Atividade Proposta

Aplique o pipeline completo em um programa **seu** (uma lista anterior, um TP, ou um algoritmo clássico como ordenação ou busca em grafos):

1. Compile com `-pg` e gere o `gmon.out`.
2. Produza o relatório textual e identifique a função mais cara (**hotspot**).
3. Gere o grafo em SVG.
4. **Otimize** a função identificada (algoritmo, estrutura de dados, ou ajuste de loop).
5. Refaça o profiling e compare **antes vs depois**.
6. Entregue um pequeno relatório (1–2 páginas) com:
   - Trecho do flat profile antes e depois.
   - Imagem do grafo antes e depois.
   - Explicação da otimização e ganho percentual.

> **Critério de avaliação:** clareza da análise > tamanho do ganho. Mesmo uma otimização modesta, bem justificada, vale mais que um ganho grande sem explicação.

---

## 11. Referências

- [GNU gprof Manual](https://sourceware.org/binutils/docs/gprof/)
- [gprof2dot — GitHub](https://github.com/jrfonseca/gprof2dot)
- [Graphviz Documentation](https://graphviz.org/documentation/)
- [Python cProfile docs](https://docs.python.org/3/library/profile.html)
- [SnakeViz](https://jiffyclub.github.io/snakeviz/)
- [py-spy — GitHub](https://github.com/benfred/py-spy)
- Brendan Gregg. *Systems Performance*, 2nd ed., Addison-Wesley, 2020.

---
