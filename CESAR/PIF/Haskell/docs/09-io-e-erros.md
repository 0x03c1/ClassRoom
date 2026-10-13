# IO e tratamento de erros

## 1. Pureza e efeitos

Funções puras calculam valores. Ações `IO` representam interações com o ambiente: ler teclado, escrever no terminal ou acessar arquivos.

```haskell
main :: IO ()
main = do
    putStrLn "Digite um valor:"
    entrada <- getLine
    putStrLn ("Você digitou: " ++ entrada)
```

`getLine` não é uma função que "retorna" uma string: ele é uma ação do tipo `IO String`. O operador `<-` obtém a `String` produzida quando a ação é executada.

## 2. Separar leitura e lógica

Para manter a lógica testável, transforme dados já validados com funções puras:

```haskell
dobro :: Int -> Int
dobro n = n * 2
```

A leitura do terminal deve ficar na borda do programa. Em aplicações maiores, essa separação facilita testes e manutenção.

## 3. Conversão segura

Evite assumir que toda entrada é um número. Uma função de conversão pode falhar.

```haskell
import Text.Read (readMaybe)

lerInteiro :: String -> Maybe Int
lerInteiro texto = do
    n <- readMaybe texto :: Maybe Integer
    if n < toInteger (minBound :: Int) || n > toInteger (maxBound :: Int)
        then Nothing
        else Just (fromInteger n)
```

`readMaybe` (do módulo `Text.Read`) devolve `Just` somente quando o texto inteiro é uma conversão válida, aceitando espaços nas pontas, e `Nothing` caso contrário.

Por que ler primeiro como `Integer`? Porque converter direto para `Int` não detecta estouro: `readMaybe "99999999999999999999" :: Maybe Int` devolve `Just 7766279631452241919`, um número que o usuário nunca digitou.

Lendo como `Integer` (precisão arbitrária) e conferindo os limites de `Int`, a entrada fora do intervalo vira `Nothing`.

## 4. Tratando `Maybe`

```haskell
dobrarEntrada :: String -> Either String Int
dobrarEntrada texto =
    case lerInteiro texto of
        Nothing -> Left "Digite um número inteiro válido."
        Just n  -> Right (n * 2)
```

## 5. Programa completo

```haskell
import Text.Read (readMaybe)

lerInteiro :: String -> Maybe Int
lerInteiro texto = do
    n <- readMaybe texto :: Maybe Integer
    if n < toInteger (minBound :: Int) || n > toInteger (maxBound :: Int)
        then Nothing
        else Just (fromInteger n)

main :: IO ()
main = do
    putStrLn "Digite um número inteiro:"
    entrada <- getLine
    case lerInteiro entrada of
        Nothing -> putStrLn "Entrada inválida."
        Just n  -> putStrLn ("Dobro: " ++ show (n * 2))
```

Salve como `Main.hs` e execute `runghc Main.hs`. O mesmo programa está em [`exemplos/EntradaSegura.hs`](../exemplos/EntradaSegura.hs).

## 6. Erros e exceções

Nem toda falha deve ser representada da mesma forma:
- `Maybe a`: algo pode estar ausente, sem necessidade de explicar o motivo.
- `Either e a`: uma operação pode falhar com uma informação de erro `e`.
- Exceções: úteis em certos limites de IO e falhas inesperadas, mas devem ser tratadas conscientemente.

## 7. Exercícios

1. Leia uma idade e informe se a entrada é válida.
2. Converta uma temperatura recebida como texto.
3. Use `Either String` para validar um nome não vazio.
4. Faça o programa repetir a pergunta até receber um inteiro válido.
