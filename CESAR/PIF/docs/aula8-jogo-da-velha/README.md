# Aula 8 — Jogo da Velha (Projeto Final)

Chegamos. Esta é a aula em que **tudo se junta**. Você vai construir um Jogo da Velha completo e jogável, usando praticamente todos os conceitos das 7 aulas anteriores:

- Janela e game loop (Aula 1)
- Formas, linhas e cores (Aula 2)
- Estado em variáveis (Aula 3)
- Vetores e atualização de estado (Aula 4)
- Reação a eventos (Aula 5)
- Estados de jogo: menu, jogando, vitória (Aula 6)
- Mouse e grade 3×3 (Aula 7)

## Objetivos

Ao final desta aula, você vai conseguir:

- Implementar um jogo completo do início ao fim.
- Detectar **condição de vitória** percorrendo linhas, colunas e diagonais.
- Detectar **empate** quando o tabuleiro está cheio.
- Alternar entre dois jogadores.
- Reiniciar a partida sem reabrir o programa.

## Análise antes de codar

Antes de qualquer linha de código, vamos pensar no problema. **Engenheiro bom pensa antes de digitar.**

### Estado que precisamos guardar

| Variável          | Tipo   | Para quê                       |
| ----------------- | ------ | ------------------------------ |
| `tabuleiro[3][3]` | `int`  | 0=vazio, 1=X, 2=O              |
| `jogadorAtual`    | `int`  | 1 ou 2                         |
| `estado`          | `enum` | `JOGANDO`, `VITORIA`, `EMPATE` |
| `vencedor`        | `int`  | 0=ninguém ainda, 1=X, 2=O      |

### Regras de transição

```
JOGANDO:
  - clique em celula vazia -> coloca peca, alterna jogador
  - apos a jogada, checar vitoria/empate
  - se alguem venceu -> estado = VITORIA
  - se ninguem venceu mas tabuleiro cheio -> estado = EMPATE

VITORIA ou EMPATE:
  - R reinicia o jogo
```

### Como detectar vitória?

Um jogador vence se preencher uma **linha**, uma **coluna** ou uma **diagonal**. São 8 possibilidades de vitória:

```
3 linhas      3 colunas      2 diagonais
─ ─ ─         | | |          \         /
─ ─ ─         | | |             \   /
─ ─ ─         | | |          /     \
```

Vamos verificar cada uma. Existem formas mais elegantes (usando arrays de combinações), mas para fins didáticos vamos manter a versão direta com `if`.

## O código

Abra [`exemplo.c`](./exemplo.c) e leia o código **inteiro** antes de compilar. Você vai reconhecer todos os conceitos. As funções principais são:

| Função                    | Responsabilidade                        |
| ------------------------- | --------------------------------------- |
| `main`                    | Game loop + máquina de estados          |
| `desenharTabuleiro`       | Desenha grade e peças                   |
| `desenharX` / `desenharO` | Desenham as peças                       |
| `verificarVitoria`        | Retorna 1, 2 ou 0 (ninguém)             |
| `tabuleiroCheio`          | Retorna `true` se não há células vazias |
| `reiniciarJogo`           | Zera tudo para uma nova partida         |

> **Repare:** dividimos o código em **funções pequenas**, cada uma com uma única responsabilidade. Esse princípio se chama **SRP — Single Responsibility Principle**. Em jogos pequenos parece exagero; em jogos grandes, é o que separa código mantível de código impossível.

## Conexão com Haskell (PIF)

Este jogo é uma excelente **ponte para a parte funcional** da disciplina. Repare em algumas observações:

- `verificarVitoria` é uma **função pura**: recebe o tabuleiro, retorna o vencedor. Não modifica nada. Funções puras são exatamente o que Haskell celebra.
- `tabuleiroCheio` também é pura.
- Já `colocarPeca` **modifica** o tabuleiro — efeito colateral. Em Haskell isso seria tratado com tipos diferentes (talvez `State` ou `IO`).

Quando você for refazer esse jogo em Haskell mais adiante na disciplina, vai perceber que as funções puras passam quase iguais. O que muda é o "loop principal" — em vez de mutação, você vai trabalhar com recursão sobre estados.

## Como compilar e rodar

- **Linux/macOS:** `./scripts/compile.sh`
- **Windows:** `scripts\compile.bat`

Clique nas células para jogar. **R** reinicia. **ESC** sai.

## Experimente

1. Mude as cores das peças. X em verde, O em rosa.
2. Troque o tamanho do tabuleiro de 600×600 para 900×900.
3. Pinte de cores diferentes as três peças que formaram a linha vencedora.
4. Adicione um som ao colocar peça (use `LoadSound` e `PlaySound` — consulte o cheatsheet).

## Exercícios — desafios para entregar

1. **Placar persistente.** Mostre o placar acumulado ao longo das partidas (`X: 3 - O: 1`) que **não zera** ao reiniciar uma rodada. Só zera ao apertar uma tecla específica (ex: F1).

2. **Ressaltar peças vencedoras.** Quando alguém vence, **destaque com um traço vermelho** as três peças que formaram a vitória. Dica: guarde quais células participaram da vitória.

3. **Tela de menu inicial.** Adicione uma `TELA_MENU` que aparece antes da partida, com botão "JOGAR". Inspire-se na Aula 6. Bônus: opção de selecionar quem começa (X ou O).

4. **Versão 4x4 ou 5x5.** Refatore o código para que o tamanho do tabuleiro seja uma **constante**. Mude para 4x4 (precisa 4 em linha para vencer) e veja o que precisa ajustar.

5. **IA simples.** Em vez de dois jogadores humanos, faça o "O" ser jogado pelo computador, escolhendo uma célula vazia aleatória. Não vai ser inteligente — mas vai ser jogável solo. Dica: `GetRandomValue(0, 8)` retorna um número aleatório.

6. **(Avançado) IA imbatível.** Implemente o algoritmo **minimax**. O computador percorre todas as jogadas possíveis e escolhe a melhor. Você sairá da disciplina sabendo o básico do que motores como o Stockfish e o AlphaZero fazem em escala gigantesca.

## Próximos passos depois desta disciplina

Você acabou de construir um jogo do zero. **Não pare aqui.** Algumas sugestões:

- **Snake** — clássico, ótimo para praticar listas encadeadas em C.
- **Breakout/Arkanoid** — extensão natural do exemplo das Aulas 4 e 5.
- **Tetris** — desafio sério com rotação de matrizes e detecção de linhas.
- **Roguelike** — usa muito grid, exatamente o que aprendemos na Aula 7.

E lembre-se: a Raylib roda **na web** via emscripten. Você pode publicar seus jogos online e mostrá-los a recrutadores, amigos e familiares. Isso vira **portfólio**.

---

## Antes de fechar este material

- [ ] Compilei e joguei a partida completa.
- [ ] X ou O ganhou pelo menos uma vez (e a tela de vitória apareceu).
- [ ] Empatei pelo menos uma vez (e a tela de empate apareceu).
- [ ] Modifiquei pelo menos **uma** coisa do código.
- [ ] Fiz pelo menos **um** dos exercícios.

Parabéns pela jornada! Foram **8 aulas, ~600 linhas de C escritas por você, um jogo completo**.

Voltar ao [README principal](../../README.md)
