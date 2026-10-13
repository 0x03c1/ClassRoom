# Configuração do Haskell no Windows

## Objetivo

Instalar o compilador GHC, o GHCi e ferramentas auxiliares. As instruções podem variar entre versões do instalador; consulte sempre o site oficial se as opções exibidas forem diferentes.

## 1. Pré-requisitos

- Windows 10 ou 11 atualizado.
- Acesso à internet.
- PowerShell.
- Permissão para instalar ferramentas no seu usuário.

Consulte: https://www.haskell.org/ghcup/

## 2. Instale pelo GHCup

Abra o site oficial do GHCup e siga as instruções específicas para Windows. O instalador pode oferecer GHC, Cabal, HLS e Stack. Para começar, priorize **GHC, Cabal e GHCup**; HLS é útil para trabalhar no VS Code.

Não execute comandos de instalação encontrados em páginas não oficiais sem verificar a origem. Leia as confirmações do instalador antes de aceitá-las.

## 3. Reabra o terminal

Após instalar, feche e abra novamente o PowerShell. Verifique:

```powershell
ghc --version
ghci --version
cabal --version
```

Se o PowerShell disser que um comando não foi encontrado:
1. Confirme se a instalação terminou sem erros.
2. Feche e reabra o terminal.
3. Confira se os diretórios de ferramentas foram adicionados ao `PATH`.
4. Consulte a documentação do GHCup antes de alterar variáveis manualmente.

## 4. Execute um comando no GHCi

```powershell
ghci
```

No prompt `ghci>`, digite:

```haskell
2 + 3
:t not
not True
```

Para sair:

```haskell
:quit
```

## 5. Crie o primeiro arquivo

Crie uma pasta para seus estudos:

```powershell
mkdir "$HOME\haskell-estudos"
cd "$HOME\haskell-estudos"
notepad Main.hs
```

No Bloco de Notas, escreva e salve:

```haskell
main :: IO ()
main = do
    putStrLn "Olá, Haskell!"
    putStrLn ("2 + 3 = " ++ show (2 + 3))
```

Confirme que o arquivo se chama `Main.hs`, não `Main.hs.txt`. No Explorador de Arquivos, habilite a exibição de extensões se necessário.

Execute:

```powershell
runghc Main.hs
```

Saída esperada:

```text
Olá, Haskell!
2 + 3 = 5
```

## 6. Compile um executável

```powershell
ghc Main.hs -o ola.exe
.\ola.exe
```

O compilador pode criar arquivos auxiliares no diretório. Eles são normais.

## 7. Editor recomendado

O Visual Studio Code é uma opção prática. Instale-o pelo site oficial e, depois, instale a extensão de Haskell que utiliza o Haskell Language Server. A disponibilidade e o nome das extensões podem mudar; confira o publisher e a documentação.

- VS Code: https://code.visualstudio.com/
- Haskell Language Server: https://haskell-language-server.readthedocs.io/

## 8. Problemas comuns

### `ghc` não é reconhecido

O executável não está disponível no `PATH` do terminal atual ou a instalação não foi concluída. Reabra o terminal e confira a instalação.

### `runghc` não é reconhecido

Verifique se o GHC foi instalado e se a ferramenta está acessível. `runghc` costuma ser distribuído com o GHC.

### O arquivo foi salvo como `.txt`

Ative a exibição das extensões de arquivo no Explorador e renomeie para `Main.hs`.

### Erro ao imprimir acentos (`invalid argument (cannot encode character)`)

Dependendo da codificação configurada no console, imprimir texto com acentos, como `"Olá"`, pode falhar com uma mensagem semelhante a `<stdout>: commitBuffer: invalid argument (cannot encode character ...)`. Duas soluções:

1. Mude a página de código do console para UTF-8 antes de executar o programa:

   ```powershell
   chcp 65001
   runghc Main.hs
   ```

2. Ou configure a codificação no próprio programa:

   ```haskell
   import System.IO (hSetEncoding, stdout, utf8)

   main :: IO ()
   main = do
       hSetEncoding stdout utf8
       putStrLn "Olá, Haskell!"
   ```

### Erro de sintaxe por indentação

Haskell usa layout. Mantenha as instruções do bloco `do` alinhadas e use espaços de forma consistente.

## Referências

- https://www.haskell.org/ghcup/
- https://www.haskell.org/
- https://downloads.haskell.org/ghc/
