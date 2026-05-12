# Aula 6 — Estado do Jogo

Todo jogo de verdade tem **telas diferentes**: um menu inicial, uma tela de jogo, uma tela de game over, talvez uma tela de pause. Cada tela responde a inputs diferentes e desenha coisas diferentes. Nesta aula você vai aprender a organizar isso usando `enum` e `switch`.

## Objetivos

Ao final desta aula, você vai conseguir:

- Modelar o estado geral do jogo com um `enum`.
- Trocar entre telas em resposta a eventos (clique, tecla, condição vencida).
- Separar a lógica de cada tela em blocos distintos.
- Entender o conceito de **máquina de estados** — fundamental para sistemas em geral, não só jogos.

## Conceito: máquina de estados

Uma **máquina de estados** é uma forma de descrever um sistema que pode estar em uma de várias "situações" diferentes, com transições bem definidas entre elas.

Para o nosso jogo, vamos ter:

```
       ENTER            quando vidas = 0
MENU ─────────► JOGANDO ──────────────────► GAME_OVER
 ▲                                             │
 │                                             │
 └──────────────────────────────────────────────┘
                 ENTER (reinicia)
```

Em C, representamos isso com um `enum`:

```c
typedef enum {
    TELA_MENU,
    TELA_JOGANDO,
    TELA_GAMEOVER
} EstadoJogo;

EstadoJogo estado = TELA_MENU;
```

E dentro do loop principal, usamos `switch` para tratar cada estado separadamente:

```c
switch (estado) {
    case TELA_MENU:      /* logica do menu */     break;
    case TELA_JOGANDO:   /* logica do jogo */     break;
    case TELA_GAMEOVER:  /* logica do game over */ break;
}
```

> **Conexão com a teoria:** isso é exatamente uma **Máquina de Estados Finita** (FSM), que você já viu ou vai ver em Teoria da Computação. Aqui você está usando o conceito na prática.

## Por que `enum` e não `int` ou `string`?

Você até **poderia** usar `int estado = 0;` (0 = menu, 1 = jogando, 2 = game over). Mas:

- Com `enum`, o **compilador te ajuda**: se você escrever `TELA_MENUU` (com erro), o compilador acusa. Com `int 0`, ninguém acusa nada.
- Com `enum`, o código fica **auto-documentado**: `case TELA_GAMEOVER` é muito mais claro que `case 2`.

Isso é uma prática essencial em C profissional. **Use `enum` sempre que tiver um conjunto fechado de opções.**

## O código

[`exemplo.c`](./exemplo.c) tem três telas:

- **MENU:** texto de boas-vindas, instrução para apertar ENTER.
- **JOGANDO:** a bola quicando da Aula 4 com um contador de tempo.
- **GAME OVER:** após 10 segundos, vai para essa tela. ENTER reinicia.

Repare em como **cada tela é um bloco bem separado** dentro do `switch`. Isso facilita demais a manutenção.

## Como compilar e rodar

- **Linux/macOS:** `./scripts/compile.sh`
- **Windows:** `scripts\compile.bat`

## Experimente

1. Adicione uma quarta tela: **PAUSE**. A partir de `TELA_JOGANDO`, ao apertar P, vai para `TELA_PAUSE`. Estando em `TELA_PAUSE`, ao apertar P, volta para `TELA_JOGANDO`.
2. Coloque um **logo** maior no menu (texto bem grande, talvez piscando — use `(int)(GetTime() * 2) % 2` para piscar).
3. Faça a tela de game over mostrar **quanto tempo durou** a partida.

## Exercícios

1. **Menu navegável.** No menu, mostre duas opções: "Iniciar" e "Sair". Use as setas para escolher (uma seta `>` ao lado da opção selecionada) e ENTER para confirmar.
2. **Splash screen.** Adicione uma tela inicial que aparece por **3 segundos** com seu nome/logo antes de ir automaticamente para o menu. Use `GetTime()` para medir o tempo.
3. **Tela de créditos.** No menu, adicione uma terceira opção: "Créditos". Ela leva a uma tela com nomes da equipe e volta ao menu ao apertar ESC.
4. **Dificuldade.** No menu, adicione uma opção de selecionar dificuldade (Fácil/Médio/Difícil) que afeta a velocidade da bola no jogo.

## Antes de ir para a próxima aula

- [ ] O jogo passa por menu → jogando → game over → menu corretamente.
- [ ] Sei explicar o que é uma máquina de estados.
- [ ] Entendi por que `enum` é melhor que `int` cru.

Proxima: [Aula 7 — Mouse e Grid](../aula7-mouse-e-grid/README.md)
