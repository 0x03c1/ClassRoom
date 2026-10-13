# Testes e qualidade

## 1. Comece com funções puras

Funções puras são fáceis de testar:

```haskell
dobro :: Int -> Int
dobro x = x * 2
```

Uma verificação manual no GHCi:

```haskell
dobro 4 == 8
dobro 0 == 0
```

## 2. Asserções simples

Para um programa didático sem dependências externas:

```haskell
dobro :: Int -> Int
dobro x = x * 2

main :: IO ()
main = do
    if dobro 4 == 8
        then putStrLn "Teste 1 passou"
        else error "Teste 1 falhou"
    if dobro 0 == 0
        then putStrLn "Teste 2 passou"
        else error "Teste 2 falhou"
```

Esse padrão serve para demonstrar a ideia, não para substituir um framework em um projeto maior.

## 3. Testes unitários e de propriedades

- **Teste unitário:** verifica um caso específico.
- **Teste de propriedade:** verifica uma regra para muitos valores gerados.
- **Teste de integração:** verifica se componentes funcionam em conjunto.

QuickCheck é uma biblioteca popular para testes baseados em propriedades. Tasty é um framework que pode reunir diferentes tipos de testes. Consulte a documentação e configure dependências pelo Cabal.

- https://hackage.haskell.org/package/QuickCheck
- https://hackage.haskell.org/package/tasty
- https://www.haskell.org/cabal/

## 4. Exemplo de propriedade

Para `dobro`, uma propriedade matemática possível é:

```haskell
dobro x == x + x
```

Um gerador de testes pode verificar essa relação para diversos inteiros. Em tipos limitados como `Int`, a propriedade acima continua coerente com a aritmética modular da implementação; escolha propriedades que façam sentido para o domínio.

## 5. Avisos do compilador

Compile com avisos:

```bash
ghc -Wall Main.hs
```

Em projetos Cabal, configure opções de avisos no arquivo do pacote. Trate avisos como oportunidades para detectar padrões incompletos, nomes não utilizados e problemas de manutenção.

## 6. Exercícios

1. Teste uma função de máximo.
2. Teste uma função que classifica números pares.
3. Escreva propriedades para `reverter (reverter xs) == xs`.
4. Teste o comportamento de uma função de conversão com entrada válida e inválida.
5. Explique por que testes de propriedade não substituem todos os testes de integração.
