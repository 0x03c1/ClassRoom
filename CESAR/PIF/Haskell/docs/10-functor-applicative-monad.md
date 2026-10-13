# Functor, Applicative e Monad

Este módulo é conceitual. Não é necessário dominar toda a teoria para escrever programas úteis; o objetivo é reconhecer as abstrações e compreender por que existem.

## 1. Contexto

Tipos como `Maybe a`, `Either e a` e `IO a` representam valores ou computações com alguma estrutura adicional. As type classes permitem definir operações genéricas sobre essas estruturas.

## 2. Functor

A ideia de `fmap` é transformar o valor dentro de uma estrutura sem alterar a estrutura externa.

```haskell
fmap (+1) (Just 4)
-- Just 5

fmap length (Just "Haskell")
-- Just 7
```

Assinatura:

```haskell
fmap :: Functor f => (a -> b) -> f a -> f b
```

Para listas, `fmap` equivale a `map`.

## 3. Applicative

`Applicative` permite aplicar uma função que está dentro de uma estrutura a valores também estruturados.

```haskell
(+) <$> Just 2 <*> Just 3
-- Just 5
```

- `<$>` é um operador infixo para `fmap`.
- `<*>` aplica uma função estruturada a um argumento estruturado.

## 4. Monad

`Monad` permite encadear computações em que a próxima etapa depende do resultado da anterior.

```haskell
Just 3 >>= (\x -> Just (x * 2))
-- Just 6
```

A operação `>>=` é chamada bind.

Uma função com `Maybe` pode encadear validações sem precisar verificar manualmente cada resultado em todas as etapas. Em `IO`, bind encadeia ações que dependem dos valores lidos.

## 5. Leis e boas práticas

As abstrações têm leis que suas instâncias devem respeitar. O nome da type class não é apenas um rótulo: as leis ajudam a garantir que operações genéricas se comportem de forma previsível.

Aprenda nesta ordem:
1. Entenda exemplos concretos com listas e `Maybe`.
2. Leia as assinaturas de tipo.
3. Pratique composição de funções.
4. Só então estude as leis formais e casos mais abstratos.

## 6. Exercícios

1. Use `fmap` para transformar um `Maybe Int`.
2. Compare `map` e `fmap` para listas.
3. Encadeie duas funções que retornam `Maybe`.
4. Explique a diferença entre transformar um valor (`fmap`) e encadear uma função que já retorna a estrutura (`>>=`).
5. Use `<$>` e `<*>` para combinar dois valores `Maybe`.
