# Haskell: do zero ao avançado

Um roteiro de aprendizagem de Haskell, em português, com configuração do ambiente em **Windows, macOS e Linux**, exemplos executáveis, exercícios e projetos progressivos.

> Objetivo: aprender Haskell com fundamentos sólidos de programação funcional, avançando até tipos algébricos, abstrações, efeitos, testes, concorrência e desempenho.

## Para quem é este material?

O material começa do zero em Haskell. Conhecimentos prévios de lógica ou de outra linguagem ajudam, mas não são obrigatórios. Cada módulo combina explicação, código e prática.

## Como usar este repositório

1. Siga a ordem dos módulos.
2. Execute os exemplos em vez de apenas lê-los. Programas prontos para rodar ficam na pasta [`exemplos/`](exemplos/).
3. Tente resolver os exercícios sozinho antes de buscar ajuda ou soluções externas.
4. Registre dúvidas e experimentos em arquivos próprios.
5. Ao final de cada etapa, desenvolva um pequeno projeto.

## Trilha de aprendizagem

| Etapa | Conteúdo | Material |
|---|---|---|
| 0 | Visão geral e plano de estudos | [Roadmap](docs/00-roadmap.md) |
| 1 | Instalação e ambiente | [Windows](docs/01-setup-windows.md) · [macOS](docs/02-setup-macos.md) · [Linux](docs/03-setup-linux.md) |
| 2 | Primeiro programa e GHCi | [Primeiros passos](docs/04-primeiros-passos.md) |
| 3 | Tipos, funções e inferência | [Tipos e funções](docs/05-tipos-e-funcoes.md) |
| 4 | Recursão, listas e pattern matching | [Recursão e listas](docs/06-recursao-e-listas.md) |
| 5 | Funções de ordem superior | [Abstrações funcionais](docs/07-ordem-superior.md) |
| 6 | Tipos algébricos e parametrização | [Modelagem de dados](docs/08-tipos-algebricos.md) |
| 7 | Entrada e saída, `Maybe`, `Either` | [Efeitos e erros](docs/09-io-e-erros.md) |
| 8 | Functors, Applicative e Monad | [Abstrações e efeitos](docs/10-functor-applicative-monad.md) |
| 9 | Testes e qualidade | [Testes](docs/11-testes.md) |
| 10 | Organização de projetos e bibliotecas | [Projetos e ferramentas](docs/12-projetos-e-ferramentas.md) |
| 11 | Concorrência, desempenho e tópicos avançados | [Avançado](docs/13-avancado.md) |
| 12 | Projetos para consolidar o aprendizado | [Projetos](docs/14-projetos.md) |
| Referência | Vocabulário essencial | [Glossário](docs/GLOSSARIO.md) |
| Prática | Lista de exercícios | [Exercícios](docs/EXERCICIOS.md) |

## Ferramentas principais

- **GHC**: compilador de Haskell.
- **GHCi**: ambiente interativo para experimentar expressões.
- **GHCup**: instalador e gerenciador de ferramentas Haskell.
- **Cabal**: ferramenta para construir projetos e gerenciar dependências.
- **HLS (Haskell Language Server)**: integração com editores, como VS Code.
- **HUnit, tasty ou QuickCheck**: opções para testes, de acordo com o projeto.

Links oficiais:
- [Haskell](https://www.haskell.org/)
- [GHCup](https://www.haskell.org/ghcup/)
- [Documentação do GHC](https://downloads.haskell.org/ghc/)
- [Cabal](https://www.haskell.org/cabal/)
- [Haskell Language Server](https://haskell-language-server.readthedocs.io/)
- [Learn You a Haskell for Great Good!](http://learnyouahaskell.com/)
- [Haskell Wikibook](https://en.wikibooks.org/wiki/Haskell)

## Primeiro teste

Crie `Main.hs`:

```haskell
main :: IO ()
main = putStrLn "Olá, Haskell!"
```

Execute com:

```bash
runghc Main.hs
```

Saída esperada:

```text
Olá, Haskell!
```

Para entrar no modo interativo:

```bash
ghci
```

No prompt `ghci>`, experimente:

```haskell
2 + 3
:t "Haskell"
:t not
```

Para sair do GHCi, use `:quit` ou `:q`.

## Princípios deste curso

- **Execute tudo:** um exemplo pequeno que funciona ensina mais do que um bloco longo que não foi testado.
- **Entenda os tipos:** eles documentam e restringem o que um programa pode fazer.
- **Modele estados inválidos para que sejam difíceis de representar:** use tipos para expressar regras do domínio.
- **Prefira clareza a esperteza:** código funcional pode ser conciso sem ser obscuro.
- **Aprenda os fundamentos antes de acumular extensões:** comece com Haskell padrão e introduza extensões quando houver uma razão clara.

## Contribuições

Correções, exemplos adicionais e melhorias didáticas são bem-vindos. Consulte [CONTRIBUTING.md](CONTRIBUTING.md).

## Licença

Este material é disponibilizado sob a licença MIT. Consulte [LICENSE](LICENSE).
