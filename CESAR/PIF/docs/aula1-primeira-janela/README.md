# Aula 1 — Primeira Janela

Bem-vindo(a) à primeira aula prática. Aqui você vai entender a **estrutura mínima** de qualquer programa Raylib. Toda animação, todo jogo, toda interação que vamos construir nas próximas aulas começa com este esqueleto.

## Objetivos

Ao final desta aula, você vai conseguir:

- Abrir uma janela com tamanho, título e taxa de atualização definidos.
- Entender o conceito de **game loop** (laço de jogo).
- Saber por que precisamos de `BeginDrawing()` e `EndDrawing()`.
- Fechar a janela corretamente.

## Conceito: o game loop

Diferente de programas comuns em C (que executam linha após linha e terminam), um jogo precisa ficar **rodando continuamente** até o usuário fechar a janela. Esse "rodar continuamente" tem nome: **game loop**.

A estrutura conceitual de um game loop é:

```
inicializa o jogo
enquanto a janela não foi fechada:
    atualiza o estado (ainda não vamos fazer isso)
    desenha tudo na tela
encerra o jogo
```

Na Raylib, isso vira:

```c
InitWindow(...);
while (!WindowShouldClose()) {
    BeginDrawing();
    // ...desenha aqui...
    EndDrawing();
}
CloseWindow();
```

> **Por que `BeginDrawing` e `EndDrawing`?** Imagine que você está pintando um quadro. `BeginDrawing` é pegar o pincel; `EndDrawing` é mostrar o quadro pronto para o público. Tudo que você desenha entre os dois é preparado em segredo e só aparece na tela depois do `EndDrawing`. Isso evita que o usuário veja o desenho "no meio do caminho".

## O código

Abra o arquivo [`exemplo.c`](./exemplo.c). Ele tem **três blocos** claramente separados por comentários:

1. **Inicialização** — criar a janela e configurar.
2. **Loop principal** — rodar até o usuário fechar.
3. **Finalização** — limpar tudo antes de sair.

## Funções da Raylib usadas

| Função | O que faz |
|--------|-----------|
| `InitWindow(largura, altura, titulo)` | Cria a janela do jogo |
| `SetTargetFPS(60)` | Limita o jogo a 60 quadros por segundo |
| `WindowShouldClose()` | Retorna `true` se o usuário clicou no X ou apertou ESC |
| `BeginDrawing()` | Prepara o quadro para receber desenhos |
| `ClearBackground(color)` | Pinta o fundo com a cor escolhida |
| `DrawText(texto, x, y, tamanho, cor)` | Desenha um texto na posição (x, y) |
| `EndDrawing()` | Mostra o quadro pronto na tela |
| `CloseWindow()` | Fecha a janela e libera memória |

## Sistema de coordenadas

Diferente da matemática que você aprendeu na escola, o **eixo Y aponta para baixo** em jogos. O canto superior esquerdo da janela é `(0, 0)` e os valores aumentam para a direita e para baixo.

```
(0,0) ────────────► X aumenta
  │
  │      Janela
  │
  ▼
 Y aumenta
```

## Como compilar e rodar

Use o script da sua plataforma na pasta `scripts/`:

- **Linux/macOS:** abra um terminal nesta pasta e rode `./scripts/compile.sh`
- **Windows:** abra um CMD nesta pasta e rode `scripts\compile.bat`

Se aparecer uma janela com fundo preto e o texto "Olá, PIF!" no meio, está tudo certo.

## Experimente quebrar o código (aprenda errando)

Tente cada uma destas alterações em `exemplo.c` antes de continuar:

1. Troque o tamanho da janela para `1200x800`.
2. Mude a cor de fundo de `BLACK` para `DARKBLUE`.
3. Mude o texto para o seu nome.
4. **O que acontece se você apagar `EndDrawing()`?** (vá em frente, teste!)
5. **O que acontece se você trocar `60` em `SetTargetFPS` por `5`?**
6. Tire o `SetTargetFPS` completamente. A janela trava? Por quê?

## Exercícios

1. Faça a janela mostrar **dois textos** em posições diferentes: seu nome no topo e o nome da disciplina embaixo.
2. Centralize o texto na janela. Dica: se a janela tem 800 de largura e seu texto tem aproximadamente 200 de largura, em que X você deve desenhar?
3. Mude o título da janela enquanto ela está aberta para `"Aula 1 - <seu nome>"`.

## Antes de ir para a próxima aula

- [ ] Compilei e rodei o `exemplo.c` sem erros.
- [ ] Modifiquei pelo menos três coisas no código e vi o resultado.
- [ ] Entendi o que é um game loop.
- [ ] Sei explicar por que existem `BeginDrawing` e `EndDrawing`.

Próxima: [Aula 2 — Formas e Cores](../aula2-formas-cores/README.md)
