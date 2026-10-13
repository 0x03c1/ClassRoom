# Configuração do Haskell no Linux

## 1. Escolha a instalação

O método recomendado para manter versões de ferramentas sob controle é consultar o GHCup:

https://www.haskell.org/ghcup/

A instalação e as dependências de sistema podem variar conforme a distribuição. Siga as instruções oficiais correspondentes à sua distribuição e versão.

## 2. Dependências de sistema

Algumas distribuições precisam de ferramentas de compilação, bibliotecas de desenvolvimento ou utilitários adicionais. Consulte os pré-requisitos atuais do GHCup para sua distribuição, em vez de reutilizar uma lista de pacotes que pode estar desatualizada.

## 3. Verifique as ferramentas

Depois da instalação, abra um novo terminal:

```bash
ghc --version
ghci --version
cabal --version
```

Se os comandos não forem encontrados, confira o `PATH` e a mensagem final do instalador.

## 4. Experimente o GHCi

```bash
ghci
```

No prompt:

```haskell
2 + 3
:t not
not False
```

Saia com `:quit`.

## 5. Crie e execute o primeiro programa

```bash
mkdir -p "$HOME/haskell-estudos"
cd "$HOME/haskell-estudos"
cat > Main.hs <<'EOF'
main :: IO ()
main = do
    putStrLn "Olá, Haskell no Linux!"
    putStrLn ("Dobro de 21: " ++ show (21 * 2))
EOF
```

Execute:

```bash
runghc Main.hs
```

Saída esperada:

```text
Olá, Haskell no Linux!
Dobro de 21: 42
```

## 6. Gere um executável

```bash
ghc Main.hs -o ola
./ola
```

## 7. Editor

VS Code com Haskell Language Server é uma opção comum:

- https://code.visualstudio.com/
- https://haskell-language-server.readthedocs.io/

## 8. Solução de problemas

- **Comando não encontrado:** reabra o terminal e confira o `PATH`.
- **Dependência ausente:** consulte os pré-requisitos da distribuição na documentação do GHCup.
- **Erro de compilação:** comece pela primeira mensagem, não pela última.
- **Permissão negada ao executar:** confira se está executando o binário correto e se ele foi gerado com sucesso.

## Referências

- https://www.haskell.org/ghcup/
- https://www.haskell.org/
- https://downloads.haskell.org/ghc/
