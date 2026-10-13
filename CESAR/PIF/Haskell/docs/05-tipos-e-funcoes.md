# Tipos, funções e inferência

## 1. Assinaturas de tipo

A assinatura informa quais tipos uma função recebe e retorna.

```haskell
dobro :: Int -> Int
dobro x = x * 2
```

`Int -> Int` descreve uma função que recebe um `Int` e retorna um `Int`.

## 2. Tipos comuns

- `Int`: inteiro de precisão limitada, conforme a implementação.
- `Integer`: inteiro de precisão arbitrária, sujeito aos limites práticos de recursos.
- `Double`: número de ponto flutuante de dupla precisão.
- `Bool`: `True` ou `False`.
- `Char`: caractere.
- `String`: sinônimo de `[Char]`, uma lista de caracteres.
- `()`: tipo unit, com um único valor também escrito `()`.

## 3. Inferência de tipos

O compilador frequentemente infere os tipos:

```haskell
triplo x = x * 3
```

O tipo inferido pode ser mais geral do que `Int -> Int`, algo semelhante a:

```haskell
triplo :: Num a => a -> a
```

`Num a =>` é uma restrição: o tipo `a` precisa oferecer as operações da type class `Num`.

Para aprender, declarar tipos explícitos nas funções principais costuma tornar o código mais legível.

## 4. Funções com mais de um argumento

```haskell
somar :: Int -> Int -> Int
somar x y = x + y
```

As setas associam à direita:

```haskell
Int -> Int -> Int
```

equivale a:

```haskell
Int -> (Int -> Int)
```

Isso significa que a função recebe um inteiro e retorna outra função que recebe um inteiro. Esse comportamento é chamado **currying**.

## 5. Aplicação parcial

```haskell
somarDez :: Int -> Int
somarDez = somar 10
```

`somar 10` produz uma função que espera o segundo argumento.

```haskell
somarDez 5
-- 15
```

## 6. Guardas e condicionais

```haskell
classificar :: Int -> String
classificar n
    | n < 0     = "negativo"
    | n == 0    = "zero"
    | otherwise = "positivo"
```

Ou com `if`:

```haskell
parOuImpar :: Int -> String
parOuImpar n =
    if even n then "par" else "ímpar"
```

Em Haskell, `if` é uma expressão: os dois ramos precisam produzir valores compatíveis.

## 7. Funções puras

Uma função pura:
- depende apenas dos seus argumentos;
- retorna o mesmo resultado para os mesmos argumentos;
- não altera estado externo nem realiza efeitos observáveis.

```haskell
quadrado :: Int -> Int
quadrado x = x * x
```

A pureza facilita raciocínio, testes e composição. Entrada e saída ficam em ações como `IO`.

## 8. Exercícios

1. Escreva `areaRetangulo :: Double -> Double -> Double`.
2. Escreva `maximoDeTres :: Int -> Int -> Int -> Int`.
3. Defina `ehMaiorDeIdade :: Int -> Bool`.
4. Escreva `aplicarDuasVezes` que recebe uma função e um valor.
5. Explique o tipo de `map` com suas próprias palavras.
