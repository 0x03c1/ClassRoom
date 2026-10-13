# Pattern matching, recursão e listas

## 1. Pattern matching

Pattern matching permite definir comportamentos a partir da forma de um valor.

```haskell
sinal :: Int -> String
sinal 0 = "zero"
sinal n
    | n < 0     = "negativo"
    | otherwise = "positivo"
```

Também é usado para decompor listas e tipos algébricos.

## 2. Listas

```haskell
numeros :: [Int]
numeros = [1, 2, 3, 4, 5]
```

`[Int]` é uma lista de inteiros. `[]` representa a lista vazia; `x : xs` representa uma cabeça `x` seguida da cauda `xs`.

```haskell
cabecaOuZero :: [Int] -> Int
cabecaOuZero [] = 0
cabecaOuZero (x:_) = x
```

Esse exemplo evita chamar `head` em uma lista vazia.

## 3. Recursão

```haskell
somaLista :: [Int] -> Int
somaLista [] = 0
somaLista (x:xs) = x + somaLista xs
```

A definição tem:
- caso-base: a lista vazia;
- caso recursivo: soma a cabeça ao resultado da cauda.

A versão recursiva é didática. Em código cotidiano, `sum` já existe.

## 4. Fatorial

```haskell
fatorial :: Integer -> Integer
fatorial n
    | n < 0     = error "fatorial não definido para negativos"
    | n == 0    = 1
    | otherwise = n * fatorial (n - 1)
```

O uso de `error` demonstra uma falha explícita, mas não é uma boa estratégia para erros esperados de entrada. Em módulos posteriores, veremos como representar falhas com `Either` ou `Maybe`.

## 5. Compreensões de listas

```haskell
quadrados :: [Int] -> [Int]
quadrados xs = [x * x | x <- xs]
```

Com filtro:

```haskell
pares :: [Int] -> [Int]
pares xs = [x | x <- xs, even x]
```

## 6. Atenção a funções parciais

`head`, `tail` e `!!` podem falhar quando recebem uma lista vazia ou um índice inválido. Prefira pattern matching ou funções que retornem `Maybe` quando a ausência de um valor for possível.

## 7. Exercícios

1. Implemente recursivamente o comprimento de uma lista.
2. Implemente `reverter` sem usar `reverse`.
3. Implemente `contarPares`.
4. Escreva uma função que retorne `Nothing` para lista vazia e `Just x` para o primeiro elemento.
5. Gere uma lista dos números de 1 a 100 divisíveis por 3 e por 5.
