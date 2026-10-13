# Tipos algébricos e modelagem de domínio

## 1. Por que criar tipos?

Tipos próprios permitem expressar as possibilidades válidas de um domínio e documentar a intenção do programa.

## 2. `data`

```haskell
data Forma
    = Circulo Double
    | Retangulo Double Double
    deriving (Show, Eq)
```

O tipo `Forma` tem duas alternativas: círculo ou retângulo.

```haskell
area :: Forma -> Double
area (Circulo raio) = pi * raio * raio
area (Retangulo largura altura) = largura * altura
```

O pattern matching trata cada construtor.

## 3. Tipos parametrizados

```haskell
data Resultado a
    = Sucesso a
    | Falha String
    deriving (Show, Eq)
```

`Resultado a` pode conter um valor de tipo `a` ou uma mensagem de falha.

## 4. `type` e `newtype`

```haskell
type Nome = String
```

`type` cria um sinônimo: `Nome` e `String` representam o mesmo tipo.

```haskell
newtype UsuarioId = UsuarioId Int
    deriving (Show, Eq)
```

`newtype` cria um tipo distinto com um único campo. Isso ajuda a evitar misturar valores semanticamente diferentes que compartilham a mesma representação.

## 5. Modelagem com `Maybe`

```haskell
buscarPrimeiro :: [a] -> Maybe a
buscarPrimeiro [] = Nothing
buscarPrimeiro (x:_) = Just x
```

A ausência de valor é explícita. Quem chama a função precisa considerar `Nothing` e `Just`.

## 6. Modelagem com `Either`

```haskell
dividir :: Double -> Double -> Either String Double
dividir _ 0 = Left "Divisão por zero"
dividir x y = Right (x / y)
```

`Left` normalmente representa falha e `Right` um resultado válido, embora o significado dependa da convenção adotada.

## 7. Exaustividade

O compilador pode alertar quando um pattern matching não cobre todos os casos. Leve esses avisos a sério: eles frequentemente revelam cenários esquecidos.

## 8. Exercícios

1. Modele um semáforo com três estados.
2. Crie um tipo para representar uma pessoa com nome e idade.
3. Modele uma forma geométrica com círculo, retângulo e triângulo.
4. Crie um tipo de resultado para validar uma senha.
5. Explique por que `newtype Email = Email String` pode ser mais seguro que usar `String` em todos os lugares.
