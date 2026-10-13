# Funções de ordem superior

Uma função de ordem superior recebe uma função como argumento ou retorna uma função.

## 1. `map`

Aplica uma função a cada elemento:

```haskell
dobro :: Int -> Int
dobro x = x * 2

map dobro [1, 2, 3, 4]
-- [2,4,6,8]
```

No GHCi, defina `dobro` antes de usá-lo (digitando a definição no prompt ou carregando um arquivo com `:load`).

Tipo:

```haskell
map :: (a -> b) -> [a] -> [b]
```

Leia `(a -> b)` como uma função que transforma um valor de tipo `a` em um valor de tipo `b`.

## 2. `filter`

Mantém elementos que satisfazem um predicado:

```haskell
filter even [1..10]
-- [2,4,6,8,10]
```

Um predicado é uma função que retorna `Bool`.

## 3. `foldr`

Reduz uma estrutura recursiva a um resultado:

```haskell
somar :: [Int] -> Int
somar = foldr (+) 0
```

O `0` é o valor inicial e `(+)` combina cada elemento com o resultado da redução.

## 4. Composição

O operador `(.)` compõe funções. Reutilizando `dobro` da seção 1:

```haskell
incrementar :: Int -> Int
incrementar x = x + 1

dobrarEIncrementar :: Int -> Int
dobrarEIncrementar = incrementar . dobro
```

`dobrarEIncrementar 3` resulta em `7`. A composição lê-se da direita para a esquerda.

## 5. Aplicação com `$`

O operador `$` aplica a função à esquerda ao valor à direita: `f $ x` é o mesmo que `f x`. Ele é útil para evitar parênteses aninhados.

```haskell
resultado = sum (map (*2) [1..5])
-- 30
```

Pode ser escrito:

```haskell
resultado = sum $ map (*2) [1..5]
```

`$` tem precedência baixa e pode reduzir parênteses. Não é obrigatório usá-lo.

## 6. Lambdas

```haskell
map (\x -> x * x) [1..5]
```

A expressão `\x -> x * x` é uma função anônima.

## 7. Exercícios

1. Use `map` para converter Celsius em Fahrenheit.
2. Use `filter` para manter palavras com mais de cinco caracteres.
3. Use `foldr` para calcular o produto de uma lista.
4. Componha funções para normalizar e depois classificar um valor.
5. Compare uma solução recursiva com uma solução que usa `map` e `filter`.
