# Primeiros passos: expressões, GHCi e `main`

## Objetivos

- Distinguir uma expressão de um programa completo.
- Usar o GHCi.
- Entender `main`, `IO ()` e `putStrLn`.
- Compilar e executar um arquivo.

## 1. Expressões

Em Haskell, uma expressão é avaliada para produzir um valor:

```haskell
2 + 3
10 * 4
2 ^ 8
```

No GHCi, digite cada expressão e observe o resultado.

## 2. Tipos de expressões

Use `:type` ou `:t`:

```haskell
:t True
:t 42
:t "Olá"
:t not
```

Uma saída como `not :: Bool -> Bool` significa que `not` recebe um `Bool` e devolve um `Bool`.

Literais numéricos podem ter tipos mais gerais no GHCi, porque o tipo pode ser inferido conforme o contexto.

## 3. Primeiro programa

```haskell
main :: IO ()
main = putStrLn "Olá, mundo!"
```

- `main` é o ponto de entrada convencional de um programa executável.
- `::` declara um tipo.
- `IO ()` representa uma ação de entrada/saída cujo resultado útil é `()`, chamado unit.
- `putStrLn` imprime texto e adiciona uma quebra de linha.

Execute com:

```bash
runghc Main.hs
```

Ou compile:

```bash
ghc Main.hs -o ola
```

## 4. Sequência com `do`

```haskell
main :: IO ()
main = do
    putStrLn "Primeira linha"
    putStrLn "Segunda linha"
```

O bloco `do` permite escrever ações de `IO` em sequência. A indentação é significativa.

## 5. Entrada de dados

```haskell
main :: IO ()
main = do
    putStrLn "Qual é o seu nome?"
    nome <- getLine
    putStrLn ("Olá, " ++ nome ++ "!")
```

O operador `<-` obtém o resultado produzido por uma ação de `IO`. Não é a mesma coisa que definir uma função ou uma constante em uma expressão pura.

## 6. Números e texto

```haskell
main :: IO ()
main = do
    let idade = 20 :: Int
    putStrLn ("Idade: " ++ show idade)
```

A anotação `:: Int` fixa o tipo do literal. Sem ela, o GHC escolhe `Integer` por padrão e, com `-Wall`, emite um aviso de *type defaulting*.

`show` converte valores que possuem uma instância de `Show` em uma representação textual.

Não se pode concatenar diretamente uma `String` e um `Int`: converta o número com `show`.

## 7. Exercícios

1. Imprima seu nome e uma frase de boas-vindas.
2. Imprima o resultado de `144 / 12`.
3. Leia dois nomes e imprima uma saudação.
4. Leia um número como texto e, depois de estudar conversões seguras, transforme-o em inteiro.
5. Consulte com `:t` os tipos de `length`, `head` e `not`.

## Erros comuns

- Esquecer aspas ao escrever uma string.
- Usar `+` para concatenar strings.
- Esquecer `show` ao misturar números e texto.
- Confundir `=` de definição com `<-` de extração de uma ação `IO`.
