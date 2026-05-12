# Aula 5 — Colisões

Movimento sem colisão é bonito mas inútil para jogos. Esta aula ensina a **detectar quando dois objetos se tocam**, abrindo a porta para tudo que vem depois: bater na raquete, pegar moeda, levar dano, ganhar ponto.

## Objetivos

Ao final desta aula, você vai conseguir:

- Detectar colisão entre dois retângulos.
- Detectar colisão entre um círculo e um retângulo.
- Detectar se o **ponto do mouse** está dentro de uma área (preparação para a Aula 7).
- Reagir a colisões: invertendo movimento, somando pontos, mudando cor.

## Conceito: bounding box

Quase todo motor de jogo (incluindo a Raylib) usa **caixas envolventes** — retângulos invisíveis ao redor de cada objeto — para detectar colisão. Essa técnica se chama **AABB** (Axis-Aligned Bounding Box). É barata, simples, e suficiente para 99% dos jogos didáticos.

```
       ┌─────────┐
       │         │     ┌─────────┐
       │   A     │     │  B      │
       │         │     │         │
       └─────────┘     └─────────┘
        Retângulo A    Retângulo B

Eles colidem? Sim, se há sobreposição em X *e* em Y.
```

A Raylib já tem essas funções prontas — você **não precisa** reimplementar a matemática.

## Funções de colisão

| Função                                           | Para que serve                  |
| ------------------------------------------------ | ------------------------------- |
| `CheckCollisionRecs(rec1, rec2)`                 | Dois retângulos se tocam?       |
| `CheckCollisionCircles(c1, r1, c2, r2)`          | Dois círculos se tocam?         |
| `CheckCollisionCircleRec(centro, raio, rec)`     | Círculo e retângulo se tocam?   |
| `CheckCollisionPointRec(ponto, rec)`             | Ponto está dentro do retângulo? |
| `CheckCollisionPointCircle(ponto, centro, raio)` | Ponto está dentro do círculo?   |

Todas retornam `bool` (`true` se colidem, `false` caso contrário).

## A struct `Rectangle`

```c
Rectangle r = { x, y, largura, altura };
```

`x` e `y` são o canto **superior esquerdo**. Mesma convenção do `DrawRectangle`.

Para desenhar um `Rectangle` já criado, use `DrawRectangleRec(r, cor)`.

## O código

[`exemplo.c`](./exemplo.c) é praticamente um **mini-Pong**: uma raquete que você controla embaixo, uma bola que cai do topo. Quando a bola toca a raquete, você ganha 1 ponto; quando ela passa direto, você perde uma vida.

Esse exemplo é uma versão simplificada do jogo que você modificou na conversa anterior — agora você vai entender **cada detalhe** do código.

## Como compilar e rodar

- **Linux/macOS:** `./scripts/compile.sh`
- **Windows:** `scripts\compile.bat`

Mova a raquete com SETAS ou A/D.

## Experimente

1. Mude o tamanho da raquete: aumente para `200` de largura, depois diminua para `60`. Sinta a diferença de dificuldade.
2. Adicione um **ângulo de rebote variável**: quando a bola bate na raquete, faça `vel.x` depender de onde bateu (`(bola.x - centro_raquete) * 0.2f`).
3. Faça a bola **acelerar** a cada acerto (multiplique `vel` por `1.05` ao bater na raquete).
4. Pinte a raquete de **verde por 5 frames** quando ela rebate a bola (feedback visual). Dica: use um contador de frames.

## Exercícios

1. **Tijolo coletável.** Adicione 3 retângulos amarelos espalhados pela tela. Quando a bola tocar qualquer um deles, ele "some" (use um array de `bool` para controlar quais ainda existem). Cada tijolo destruído vale 10 pontos.
2. **Inimigo.** Adicione um retângulo vermelho que se move horizontalmente em cima da tela. Se a bola tocar nele, **game over** instantâneo.
3. **Power-up.** Crie um círculo verde que aparece em posição aleatória a cada 5 segundos. Se a bola pegar, a raquete aumenta de tamanho por 10 segundos.
4. **Detecção de mouse.** Adicione um botão "RESET" na tela. Use `CheckCollisionPointRec(GetMousePosition(), botaoRec)` para detectar quando o usuário clica nele com o mouse e zera o score.

## Antes de ir para a próxima aula

- [ ] Vi a bola sendo rebatida pela raquete.
- [ ] Sei explicar o que é uma "bounding box".
- [ ] Modifiquei o código para reagir de jeito diferente quando há colisão.

Proxima: [Aula 6 — Estado do Jogo](../aula6-estado-do-jogo/README.md)
