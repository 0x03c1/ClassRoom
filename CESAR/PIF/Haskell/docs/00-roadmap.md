# Roadmap: do zero ao avançado

## Como estudar

Uma rotina sugerida é estudar de 30 a 60 minutos por sessão, quatro ou cinco vezes por semana. Ajuste o ritmo: avançar com compreensão é mais importante que cumprir um calendário rígido.

## Fase 1 — Fundamentos

**Objetivos**
- Instalar GHC e GHCi.
- Executar arquivos `.hs`.
- Entender expressões, funções e tipos.
- Ler mensagens do compilador.

**Critério de conclusão:** escrever funções simples, explicar seus tipos e executar um programa sem copiar um tutorial.

## Fase 2 — Pensamento funcional

- Imutabilidade e ausência de atribuição tradicional.
- Funções puras e efeitos.
- Pattern matching.
- Recursão e listas.
- `map`, `filter`, `foldr` e composição.

**Critério de conclusão:** transformar uma sequência de dados com funções pequenas e compor essas funções.

## Fase 3 — Modelagem e tratamento de falhas

- `data`, `newtype` e `type`.
- Tipos parametrizados.
- `Maybe` e `Either`.
- Modelagem de domínio com tipos algébricos.
- Separação entre lógica pura e entrada/saída.

**Critério de conclusão:** modelar um problema real sem representar erros com valores mágicos, como `-1` ou string vazia.

## Fase 4 — Abstrações

- Type classes.
- `Functor`, `Applicative` e `Monad`.
- `foldMap`, `traverse` e abstrações relacionadas.
- Leitura de assinaturas de tipos mais gerais.

**Critério de conclusão:** explicar a intenção de uma abstração e reconhecer seu uso em código, sem precisar memorizar todas as instâncias.

## Fase 5 — Engenharia de software

- Cabal e estrutura de pacotes.
- Testes unitários e testes baseados em propriedades.
- Documentação e integração contínua.
- Profiling, strictness e estruturas de dados.
- Concorrência e paralelismo quando necessários.

**Critério de conclusão:** construir um pacote pequeno com testes, documentação e instruções reproduzíveis.

## Fase 6 — Especialização

Escolha uma trilha:
- Backend e APIs.
- Compiladores e linguagens.
- Ciência de dados e processamento de texto.
- Concorrência e sistemas.
- Verificação de propriedades e programação segura.
- Pesquisa em semântica, tipos e lógica.

## Calendário ilustrativo de 12 semanas

| Semana | Foco |
|---|---|
| 1 | Instalação, GHCi, expressões |
| 2 | Tipos, funções e currying |
| 3 | Pattern matching e guardas |
| 4 | Recursão e listas |
| 5 | Funções de ordem superior |
| 6 | Tipos algébricos e modelagem |
| 7 | `Maybe`, `Either` e `IO` |
| 8 | Type classes |
| 9 | Functor, Applicative e Monad |
| 10 | Cabal e testes |
| 11 | Desempenho e concorrência |
| 12 | Projeto final |

Esse calendário é uma referência, não uma promessa de domínio completo em 12 semanas. Os temas avançados merecem prática continuada.
