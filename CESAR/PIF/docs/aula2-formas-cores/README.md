# Aula 2 — Formas e Cores

Agora que você sabe abrir uma janela, vamos **preenchê-la com elementos visuais**: retângulos, círculos, linhas, texto e cores. Tudo o que você vê em um jogo é construído com formas geométricas básicas — até o sprite mais elaborado começa como um retângulo.

## Objetivos

Ao final desta aula, você vai conseguir:

- Desenhar formas geométricas (retângulo, círculo, linha, triângulo) na tela.
- Usar o sistema de cores da Raylib e criar suas próprias cores.
- Posicionar formas com precisão usando o sistema de coordenadas.
- Entender a diferença entre formas "preenchidas" e formas "só com borda".

## Conceito: tudo é coordenada

Cada forma que você desenha precisa saber **onde** está. Na Raylib, isso é feito de duas maneiras:

- **Retângulos** são posicionados pelo canto **superior esquerdo**.
- **Círculos** são posicionados pelo **centro**.

```
Retângulo em (100, 50)         Círculo em (300, 100), raio 40
com largura 80 e altura 40

  (100,50)                              (300,100)
     ┌─────────┐                            ◯
     │         │
     │         │ 40
     │         │
     └─────────┘
          80
```

## Cores na Raylib

A Raylib oferece um conjunto de cores pré-definidas que você pode usar diretamente:

```
BLACK, WHITE, RAYWHITE (branco levemente acinzentado, padrao da lib),
GRAY, DARKGRAY, LIGHTGRAY,
RED, MAROON,
GREEN, LIME, DARKGREEN,
BLUE, DARKBLUE, SKYBLUE,
YELLOW, GOLD, ORANGE,
PURPLE, VIOLET, MAGENTA, PINK,
BROWN, BEIGE
```

Mas você **não está limitado** a elas. Pode criar suas próprias cores com `Color` ou a macro `CLITERAL`:

```c
Color minhaCor = { 50, 150, 200, 255 };  // R, G, B, Alpha (transparencia)
```

Cada componente vai de **0 a 255**:
- `R` = quanto de vermelho
- `G` = quanto de verde
- `B` = quanto de azul
- `A` = transparência (0 = totalmente transparente, 255 = totalmente opaco)

> **Curiosidade:** essa representação é a mesma usada em CSS, Photoshop e quase tudo no mundo digital. Aprendendo aqui, você aprendeu para a vida.

## Funções de desenho que vamos usar

| Função                                           | Para que serve                |
| ------------------------------------------------ | ----------------------------- |
| `DrawRectangle(x, y, largura, altura, cor)`      | Retângulo preenchido          |
| `DrawRectangleLines(x, y, largura, altura, cor)` | Retângulo só com borda        |
| `DrawCircle(x, y, raio, cor)`                    | Círculo preenchido            |
| `DrawCircleLines(x, y, raio, cor)`               | Círculo só com borda          |
| `DrawLine(x1, y1, x2, y2, cor)`                  | Linha de um ponto ao outro    |
| `DrawTriangle(v1, v2, v3, cor)`                  | Triângulo entre três vértices |
| `DrawText(texto, x, y, tamanho, cor)`            | Texto                         |
| `GetScreenWidth()` / `GetScreenHeight()`         | Tamanho atual da janela       |

## O código

Abra [`exemplo.c`](./exemplo.c). Ele desenha um **rosto simples** (cabeça, olhos, boca) usando só as funções acima.

Repare que **a ordem das chamadas importa**: o que é desenhado depois fica **por cima** do que veio antes. Pense em camadas, como folhas empilhadas.

## Como compilar e rodar

Mesma coisa de antes:

- **Linux/macOS:** `./scripts/compile.sh`
- **Windows:** `scripts\compile.bat`

## Experimente

1. Mude a cor do fundo de `RAYWHITE` para uma cor que você criou com `Color`.
2. Adicione **orelhas** ao rosto (dois círculos nas laterais da cabeça).
3. Mude a boca de uma linha para um **arco** usando `DrawCircleSector` (consulte o [cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)).
4. Inverta a ordem em que os olhos são desenhados em relação à cabeça. O que acontece?

## Exercícios

1. **Bandeira do Brasil simplificada.** Desenhe um retângulo verde ocupando a tela inteira, um losango amarelo no meio (use 4 triângulos ou `DrawPoly`), e um círculo azul no centro.
2. **Casa simples.** Desenhe uma casa usando apenas retângulos, triângulos e linhas: parede, telhado, porta, janela.
3. **Gradiente.** Use `DrawRectangleGradientV` para desenhar um céu com gradiente do azul claro (em cima) para o azul escuro (embaixo). Consulte o cheatsheet.
4. **Identidade visual.** Crie uma "logo" sua misturando formas. Será sua marca em todos os jogos da disciplina.

## Antes de ir para a próxima aula

- [ ] Desenhei pelo menos 5 formas diferentes na tela.
- [ ] Criei pelo menos uma cor customizada.
- [ ] Entendi que a ordem das chamadas afeta o resultado visual.

Proxima: [Aula 3 — Input do Teclado](../aula3-input-teclado/README.md)
