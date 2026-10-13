# Projetos, Cabal e organização

## 1. Quando sair de arquivos isolados

Arquivos `.hs` são suficientes para aprender sintaxe e testar funções pequenas. Para projetos reais, use uma ferramenta de build e gerenciamento de dependências.

O Cabal permite descrever um pacote, dependências, bibliotecas, executáveis e testes.

Documentação: https://www.haskell.org/cabal/

## 2. Criar um projeto

Confira a versão instalada:

```bash
cabal --version
```

Consulte `cabal help` para os comandos disponíveis na sua versão. Um fluxo comum para iniciar um projeto é:

```bash
cabal init
```

O assistente pode perguntar nome, tipo de pacote e outros metadados. Depois, examine os arquivos gerados antes de executar build ou testes.

## 3. Estrutura conceitual

Um projeto pode ter:
- `app/`: ponto de entrada do executável.
- `src/`: módulos da aplicação.
- `test/`: testes.
- arquivo `.cabal`: metadados, componentes e dependências.
- `README.md`: objetivo, instalação e uso.

A estrutura exata depende do projeto e do template escolhido pelo Cabal.

## 4. Módulos

Arquivo `Calculadora.hs`:

```haskell
module Calculadora (somar, multiplicar) where

somar :: Int -> Int -> Int
somar x y = x + y

multiplicar :: Int -> Int -> Int
multiplicar x y = x * y
```

A lista entre parênteses após o nome do módulo exporta apenas as funções indicadas.

## 5. Controle de versão

Registre o código com Git. Evite versionar artefatos de build desnecessários; confira o `.gitignore` apropriado ao projeto.

```bash
git init
git add README.md
git commit -m "Adiciona documentação inicial"
```

## 6. Documentação

Um bom README deve explicar:
- problema que o projeto resolve;
- requisitos;
- como instalar dependências;
- como executar;
- como testar;
- limitações conhecidas.

## 7. Próximos passos

Depois de se familiarizar com Cabal, explore HLS para integração com editor, Haddock para documentação de módulos e integração contínua para automatizar builds e testes.
