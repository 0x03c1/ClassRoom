# Aula 4 — Movimento e Física Simples

Até agora você moveu coisas **pelo teclado**. Agora vamos fazer coisas se moverem **sozinhas**, com física básica: velocidade, direção e colisão com as paredes da janela. Essa é a base de qualquer bola, projétil, inimigo ou partícula em um jogo.

## Objetivos

Ao final desta aula, você vai conseguir:

- Representar **posição** e **velocidade** como `Vector2`.
- Atualizar a posição de um objeto a cada frame.
- Fazer um objeto **quicar** nas bordas da tela.
- Entender por que `float` (e não `int`) é melhor para movimento.

## Conceito: posição + velocidade

A regra mais antiga da física aparece em todo jogo:

```
nova_posicao = posicao_atual + velocidade
```

Aplicamos essa fórmula **uma vez por frame** (ou seja, 60 vezes por segundo a 60 FPS). Se a velocidade for `(5, 3)`, em um segundo o objeto andou `(300, 180)` pixels.

Em código:

```c
Vector2 posicao  = { 100, 100 };
Vector2 velocidade = { 5, 3 };

// dentro do loop:
posicao.x += velocidade.x;
posicao.y += velocidade.y;
```

## Quicando nas bordas

Para fazer um objeto **rebater** na borda, invertemos o sinal do componente correspondente da velocidade:

```
se bateu na borda esquerda ou direita -> velocidade.x = -velocidade.x
se bateu na borda de cima ou de baixo -> velocidade.y = -velocidade.y
```

Imagine uma bola indo para a direita com `vx = 5`. Quando ela bate na parede direita, viramos para `vx = -5` e ela passa a ir para a esquerda. Esse é literalmente o trecho de física do **Pong** (1972), um dos primeiros videogames da história.

## Por que `float` e não `int`?

Velocidade real em jogos quase nunca é número inteiro. Imagine uma bola se movendo a `3.7` pixels por frame, em ângulo de 30 graus: o movimento horizontal seria `3.7 * cos(30°) ≈ 3.2`. Se usarmos `int`, perdemos esses decimais e o movimento fica "tremido" e impreciso.

Por isso, na Raylib, posições de objetos em movimento usam `Vector2` (que é `struct { float x; float y; }`). Convertemos para inteiro **só na hora de desenhar**, se necessário.

## O código

[`exemplo.c`](./exemplo.c) tem uma bola quicando livremente na tela, mais um quadrado controlado pelo jogador. A bola muda de cor toda vez que bate em uma parede.

## Como compilar e rodar

- **Linux/macOS:** `./scripts/compile.sh`
- **Windows:** `scripts\compile.bat`

## Experimente

1. Aumente a velocidade da bola para `(15, 12)`. Está visivelmente "tremida"? Por que ainda parece OK?
2. Mude a velocidade da bola para `(0, 0)`. Ela para. Agora aperte a tecla **ESPAÇO** dentro do código (adicione você mesmo) para dar um "chute" inicial.
3. Adicione **gravidade**: a cada frame, some `0.3` ao `velocidade.y`. O que acontece?
4. Combine gravidade com rebote — mas em vez de `vy = -vy` quando bate no chão, faça `vy = -vy * 0.8`. Bola que perde energia. Reconhece esse efeito?

## Exercícios

1. **Mais bolas.** Crie um **array de 5 bolas**, cada uma com posição e velocidade aleatórias (use `GetRandomValue(min, max)`). Atualize e desenhe todas em um `for`.
2. **Bola que segue o mouse.** Em vez de velocidade fixa, calcule a velocidade como `(mouse_x - bola_x) * 0.05`. Use `GetMouseX()` e `GetMouseY()`. A bola passa a ser "atraída" pelo cursor.
3. **Mini-asteroides.** Faça 10 círculos pequenos quicando, todos com cores diferentes. Cada vez que algum bate em parede, troque a cor.
4. **Trail / rastro.** Em vez de `ClearBackground` total, desenhe um retângulo semi-transparente sobre toda a tela. A bola deixa rastro!

## Antes de ir para a próxima aula

- [ ] Entendi que `posicao += velocidade` é a base de qualquer movimento.
- [ ] Vi a bola quicar nas 4 paredes.
- [ ] Modifiquei a velocidade e vi o efeito.
- [ ] Sei explicar por que usamos `float` e não `int` para movimento.

Proxima: [Aula 5 — Colisoes](../aula5-colisoes/README.md)
