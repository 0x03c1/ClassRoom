# Raylib PIF — Construindo Jogos em C

Bem-vindo(a)! Este repositório é o material prático da disciplina de **Programação Imperativa e Funcional (PIF)** para a parte de C.

A ideia é simples: você vai aprender a biblioteca **Raylib** do zero, passo a passo, e ao final terá construído um **Jogo da Velha completo** com as próprias mãos.

---

## O que é a Raylib?

A biblioteca `raylib.h` é uma biblioteca de programação de jogos em **C puro**, criada por Ramon Santamaria. Ela foi pensada para ser **simples, didática e poderosa**, e é usada tanto para ensino quanto para jogos comerciais publicados em lojas como Steam e itch.io.

Diferente de bibliotecas como SDL ou OpenGL, que exigem dezenas de linhas só para abrir uma janela, com a Raylib você consegue ter uma janela animada e interativa em **menos de 15 linhas de código**. Isso permite que você foque no que importa nesta disciplina: **lógica, estado e estruturas de dados**.

### Por que estamos usando Raylib na disciplina?

- **API em C puro** — sem mágica de C++, sem orientação a objeto. Tudo é função, struct e ponteiro.
- **Nomes auto-explicativos** — `DrawCircle`, `IsKeyPressed`, `CheckCollisionRecs`. Você lê e entende.
- **Multiplataforma de verdade** — o mesmo código roda em Windows, Linux, macOS, no navegador (WebAssembly) e até Android.
- **Excelente para visualizar conceitos imperativos** — loop principal, mutação de estado, condicionais e arrays aparecem naturalmente.

---

## Antes de começar

Você precisa configurar o ambiente na sua máquina. Siga o guia de instalação:

**[SETUP.md — Como instalar a Raylib no seu sistema](./SETUP.md)**

Só prossiga para as aulas depois de conseguir compilar e rodar o exemplo final do SETUP.

---

## Trilha de aulas

Cada aula tem um `README.md` próprio com a explicação, um `exemplo.c` para você compilar e modificar, e scripts de compilação para Windows, Linux e macOS.

**Faça as aulas em ordem.** Cada uma usa conceitos da anterior.

| # | Aula | O que você vai aprender |
|---|------|-------------------------|
| 1 | [Primeira Janela](./docs/aula1-primeira-janela/README.md) | Estrutura mínima de um programa Raylib, game loop, FPS |
| 2 | [Formas e Cores](./docs/aula2-formas-cores/README.md) | Desenhar retângulos, círculos, linhas, texto e usar o sistema de cores |
| 3 | [Input do Teclado](./docs/aula3-input-teclado/README.md) | Detectar teclas, diferença entre tecla pressionada e tecla segurada |
| 4 | [Movimento e Física Simples](./docs/aula4-movimento-fisica/README.md) | Mover objetos na tela, vetores, velocidade e quicar nas bordas |
| 5 | [Colisões](./docs/aula5-colisoes/README.md) | Detectar quando dois objetos se tocam, funções de colisão da Raylib |
| 6 | [Estado do Jogo](./docs/aula6-estado-do-jogo/README.md) | Telas (menu, jogando, game over), `enum` e `switch` |
| 7 | [Mouse e Grid](./docs/aula7-mouse-e-grid/README.md) | Capturar cliques do mouse, desenhar grade, converter pixel em coordenada |
| 8 | [Jogo da Velha Completo](./docs/aula8-jogo-da-velha/README.md) | **Projeto final** — juntar tudo num jogo jogável |

---

## Como estudar com este material

1. **Leia o README da aula primeiro**, sem ir direto para o código.
2. Abra o `exemplo.c` e tente prever o que ele faz **antes** de compilar.
3. Compile e rode usando o script da sua plataforma (`compile.sh` para Linux/macOS, `compile.bat` para Windows).
4. **Modifique o código.** Mude cores, valores, comportamentos. Quebre o programa de propósito e veja o que acontece.
5. Faça os **exercícios** ao final de cada README antes de avançar para a próxima aula.

> **Dica do professor:** o aluno que mais aprende não é o que copia o código, é o que **quebra o código** e tenta consertar. Erre à vontade.

---

## Material de apoio

- Site oficial da Raylib: <https://www.raylib.com>
- Cheatsheet com todas as funções: <https://www.raylib.com/cheatsheet/cheatsheet.html>
- Exemplos oficiais (mais de 100): <https://www.raylib.com/examples.html>

---

## Créditos

Material produzido para a disciplina **PIF — Programação Imperativa e Funcional**.
