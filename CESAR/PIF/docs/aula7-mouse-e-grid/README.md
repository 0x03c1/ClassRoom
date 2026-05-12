# Aula 7 — Mouse e Grid

Estamos chegando perto do projeto final. Para fazer o Jogo da Velha, precisamos de duas coisas que ainda não vimos: **capturar cliques do mouse** e **trabalhar com uma grade (grid)**, que é a estrutura natural do tabuleiro 3x3.

## Objetivos

Ao final desta aula, você vai conseguir:

- Capturar a posição do mouse e detectar cliques.
- Desenhar uma grade na tela usando `for`.
- Converter coordenada do pixel em coordenada do grid (e vice-versa).
- Trabalhar com **matrizes** (`int matriz[3][3]`) — peça central do jogo da velha.

## Conceito: pixel para grid

Imagine uma grade 3×3 cobrindo uma janela de 600×600. Cada célula tem 200×200 pixels.

```
   0     200    400    600
0  ┌─────┬─────┬─────┐
   │ 0,0 │ 0,1 │ 0,2 │
200├─────┼─────┼─────┤
   │ 1,0 │ 1,1 │ 1,2 │
400├─────┼─────┼─────┤
   │ 2,0 │ 2,1 │ 2,2 │
600└─────┴─────┴─────┘
```

Quando o usuário clica no pixel `(350, 280)`, em qual célula ele clicou?

**Resposta:**
- coluna = `350 / 200` = `1`
- linha = `280 / 200` = `1`
- célula = `(1, 1)` — o centro!

Essa divisão inteira é o **truque mais comum** em jogos de tabuleiro. Memorize:

```c
int coluna = mouseX / tamanhoCelula;
int linha  = mouseY / tamanhoCelula;
```

Para o caminho inverso (qual pixel é o canto da célula `(linha, coluna)`):

```c
int pixelX = coluna * tamanhoCelula;
int pixelY = linha  * tamanhoCelula;
```

## Funções de mouse na Raylib

| Função                                    | O que faz                                    |
| ----------------------------------------- | -------------------------------------------- |
| `GetMousePosition()`                      | Retorna `Vector2` com posição atual do mouse |
| `GetMouseX()` / `GetMouseY()`             | Retornam `int` com X ou Y                    |
| `IsMouseButtonPressed(MOUSE_BUTTON_LEFT)` | `true` no frame em que o usuário clicou      |
| `IsMouseButtonDown(MOUSE_BUTTON_LEFT)`    | `true` enquanto o botão estiver segurado     |

Botões disponíveis: `MOUSE_BUTTON_LEFT`, `MOUSE_BUTTON_RIGHT`, `MOUSE_BUTTON_MIDDLE`.

## Matrizes em C

Para representar o tabuleiro do jogo da velha, usamos uma matriz 3×3 de inteiros:

```c
int tabuleiro[3][3] = {
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 }
};
```

Convenção que vamos usar:
- `0` = célula vazia
- `1` = X (jogador 1)
- `2` = O (jogador 2)

Para acessar uma célula:

```c
tabuleiro[linha][coluna] = 1;
```

> **Cuidado em C:** matrizes NÃO são checadas em runtime. Se você acessar `tabuleiro[5][7]`, o compilador não reclama e o programa pode até rodar — mas você está lendo memória inválida. **Sempre valide os índices antes**.

## O código

[`exemplo.c`](./exemplo.c) desenha uma grade 3×3 e destaca a célula sobre a qual o mouse está. Ao clicar, a célula muda de cor permanentemente. Clicar de novo limpa.

Este código é, conceitualmente, **90% do jogo da velha**. Na próxima aula só precisaremos adicionar X e O, alternar jogadores, e detectar vitória.

## Como compilar e rodar

- **Linux/macOS:** `./scripts/compile.sh`
- **Windows:** `scripts\compile.bat`

## Experimente

1. Mude a grade para 4×4 ou 5×5. Tudo que você precisa mudar é uma constante e o tamanho da matriz.
2. Pinte cada célula com uma cor diferente baseada em `(linha + coluna) % 2` — isso cria um padrão de tabuleiro de xadrez.
3. Adicione um contador de cliques no canto da tela.
4. Faça o botão **direito** do mouse limpar todas as células (use um `for` aninhado).

## Exercícios

1. **Tabuleiro de xadrez.** Pinte as 64 casas de um tabuleiro 8×8 em padrão xadrez (preto e branco alternando).
2. **Campo Minado simplificado.** Em cada célula clicada com o esquerdo, mostre um número aleatório de 1 a 8. Com o direito, marque com uma bandeira (desenhe um triângulo vermelho).
3. **Painel de pixels.** Crie uma grade de 20×20 onde cada clique pinta a célula. Diferentes botões do mouse pintam de cores diferentes. Você acaba de criar um mini editor de pixel art!
4. **Hover com cores.** A célula sob o mouse muda de cor suavemente. Use `Fade(cor, 0.5f)` para semi-transparência.

## Antes de ir para a próxima aula

- [ ] Cliquei em diferentes células e vi a cor mudar.
- [ ] Entendi a conversão `pixel → grid` (divisão inteira) e `grid → pixel` (multiplicação).
- [ ] Sei como declarar e acessar uma matriz `int [3][3]` em C.

Proxima: [Aula 8 — Jogo da Velha](../aula8-jogo-da-velha/README.md) — **projeto final!**
