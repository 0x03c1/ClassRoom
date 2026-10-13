# Configuração do Haskell no macOS

## 1. Pré-requisitos

- Uma versão suportada do macOS.
- Terminal.
- Conexão com a internet.

O GHCup é o caminho recomendado para gerenciar ferramentas Haskell: https://www.haskell.org/ghcup/

## 2. Instale as ferramentas

Abra a página oficial do GHCup e siga as instruções atualizadas para macOS. O método de instalação pode depender da versão do sistema e da arquitetura do processador (Apple Silicon ou Intel).

Se o instalador solicitar componentes adicionais, leia a documentação e os avisos apresentados. Não copie comandos de instalação de fontes desconhecidas.

## 3. Verifique a instalação

Feche e abra o Terminal novamente:

```bash
ghc --version
ghci --version
cabal --version
```

Se os comandos não forem encontrados, confira as mensagens do instalador e a configuração do `PATH` conforme a documentação do GHCup.

## 4. Teste o GHCi

```bash
ghci
```

Digite:

```haskell
2 + 3
:t "Haskell"
:t not
```

Saia com:

```haskell
:quit
```

## 5. Crie um programa

```bash
mkdir -p "$HOME/haskell-estudos"
cd "$HOME/haskell-estudos"
cat > Main.hs <<'EOF'
main :: IO ()
main = do
    putStrLn "Olá, Haskell no macOS!"
    putStrLn ("Quadrado de 7: " ++ show (7 * 7))
EOF
```

Execute:

```bash
runghc Main.hs
```

Saída esperada:

```text
Olá, Haskell no macOS!
Quadrado de 7: 49
```

## 6. Compile

```bash
ghc Main.hs -o ola
./ola
```

## 7. Editor

O VS Code com Haskell Language Server oferece realce de sintaxe, diagnósticos e navegação de código.

- https://code.visualstudio.com/
- https://haskell-language-server.readthedocs.io/

## 8. Diagnóstico rápido

- `command not found`: reabra o terminal e verifique o `PATH`.
- Falha de instalação: consulte a documentação do GHCup para sua versão do macOS e arquitetura.
- Código não compila: leia a primeira mensagem de erro; erros posteriores podem ser consequência do primeiro.

## Referências

- https://www.haskell.org/ghcup/
- https://www.haskell.org/
