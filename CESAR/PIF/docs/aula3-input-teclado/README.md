# Aula 3 — Input do Teclado

Até agora a janela só mostrava coisas. Nesta aula, o usuário **conversa de volta** com o programa pelo teclado. Esse é o primeiro passo para qualquer jogo de verdade.

## Objetivos

Ao final desta aula, você vai conseguir:

- Detectar se uma tecla está sendo pressionada **agora**.
- Diferenciar tecla "pressionada uma vez" de tecla "sendo segurada".
- Mover um objeto na tela controlado pelo usuário.
- Identificar teclas tanto por seta quanto por letra (WASD).

## Conceito: dois tipos de input

A Raylib oferece duas funções principais para detectar teclas:

| Função                | Comportamento                                                     |
| --------------------- | ----------------------------------------------------------------- |
| `IsKeyDown(KEY_X)`    | Retorna `true` **enquanto** a tecla estiver segurada              |
| `IsKeyPressed(KEY_X)` | Retorna `true` **apenas no frame** em que a tecla foi pressionada |

Por que isso importa? Pense em dois exemplos:

- **Mover um personagem:** você quer que ele continue andando enquanto a seta está segurada → `IsKeyDown`.
- **Pular:** você quer que pule **uma vez** mesmo que o usuário segure a tecla → `IsKeyPressed`.

> **Analogia:** `IsKeyDown` é "está pisando no acelerador"; `IsKeyPressed` é "deu um toque no botão da campainha".

## Constantes de teclas

Algumas das mais comuns:

```
KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN   -- setas
KEY_W, KEY_A, KEY_S, KEY_D              -- letras (todas em maiusculo)
KEY_SPACE, KEY_ENTER, KEY_ESCAPE, KEY_TAB
KEY_ZERO, KEY_ONE, ..., KEY_NINE        -- numeros
KEY_F1, KEY_F2, ..., KEY_F12
```

A lista completa está no [cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html).

## O código

Em [`exemplo.c`](./exemplo.c) você tem um **quadrado controlado pelo jogador**. Use setas ou WASD para mover, ESPAÇO para mudar de cor.

Note como o estado (`x`, `y`, `cor`) é **declarado uma vez antes do loop** e **modificado dentro dele**. Esse é o coração da programação imperativa: variáveis que mudam ao longo do tempo. Guarde essa observação — quando você for ver Haskell, vai entender por que esse modelo é o oposto do paradigma funcional.

## Velocidade independente do hardware?

Você vai ver no código que somamos `5` à posição a cada frame. Como rodamos a 60 FPS, isso significa **300 pixels por segundo**. Se o jogo rodar mais rápido em algum hardware, o personagem andaria mais rápido também — o que é ruim.

Em jogos profissionais, multiplicamos por `GetFrameTime()` (tempo desde o último frame) para garantir velocidade constante independente do FPS:

```c
float velocidade = 300.0f; // pixels por segundo
x += velocidade * GetFrameTime();
```

Não vamos usar isso nas próximas aulas para manter o código simples, mas saiba que **é assim que jogos profissionais fazem**.

## Como compilar e rodar

- **Linux/macOS:** `./scripts/compile.sh`
- **Windows:** `scripts\compile.bat`

Use as setas ou WASD para mover o quadrado. Aperte ESPAÇO para mudar a cor.

## Experimente

1. Aumente a velocidade de `5` para `15`. O quadrado fica mais rápido.
2. Troque `IsKeyDown` por `IsKeyPressed` no movimento. Como o controle muda? Por quê?
3. Troque `IsKeyPressed` por `IsKeyDown` na troca de cor. O que acontece?
4. Adicione um contador de quantas vezes a cor foi trocada. Use `DrawText` com `TextFormat("Trocas: %d", contador)`.

## Exercícios

1. **Limites da tela.** Impeça o quadrado de sair pelas bordas. Se `x` for menor que 0, mantenha em 0. Se for maior que `largura - tamanho`, mantenha no limite.
2. **Velocidade variável.** Faça o quadrado andar **mais devagar** quando SHIFT estiver segurado e **mais rápido** quando CTRL estiver. Dica: tem `KEY_LEFT_SHIFT` e `KEY_LEFT_CONTROL`.
3. **Duas teclas iguais a uma diagonal.** O movimento já funciona em diagonal porque `if` é independente. Mas note: o quadrado anda mais rápido na diagonal! Por quê? (Pesquise "movimento normalizado".)
4. **Múltiplos quadrados.** Crie um segundo quadrado controlado por IJKL. Dois jogadores no mesmo teclado.

## Antes de ir para a próxima aula

- [ ] Movi o quadrado em todas as direções.
- [ ] Entendi a diferença entre `IsKeyDown` e `IsKeyPressed`.
- [ ] Sei explicar por que o estado é declarado **fora** do loop.

Proxima: [Aula 4 — Movimento e Fisica Simples](../aula4-movimento-fisica/README.md)
