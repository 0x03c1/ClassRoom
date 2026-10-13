# Tópicos avançados

Este módulo é um mapa para estudo continuado. Cada tema merece exemplos, leituras e experimentação; não tente aprender tudo de uma vez.

## 1. Type classes e instâncias

Type classes definem conjuntos de operações e restrições sobre tipos. Você já viu exemplos como `Eq`, `Show` e `Num`.

```haskell
data Prioridade = Baixa | Media | Alta
    deriving (Show, Eq, Ord)
```

Deriving automático é adequado para classes comuns quando a semântica padrão faz sentido. Em domínios mais complexos, uma instância manual pode ser necessária.

Estude:
- declarar type classes;
- implementar instâncias;
- restrições em assinaturas;
- leis das abstrações;
- newtypes e derivação.

## 2. Tipos avançados

Temas posteriores:
- GADTs;
- famílias de tipos;
- tipos existenciais;
- tipos de ordem superior;
- kinds;
- tipos dependentes em extensões e linguagens relacionadas;
- abstrações com `RankNTypes`.

Esses recursos podem exigir extensões do GHC. Declare as extensões utilizadas e verifique a documentação da versão do compilador.

## 3. Strictness e avaliação preguiçosa

Haskell usa avaliação não estrita por padrão. Uma expressão pode ser avaliada somente quando seu resultado é necessário.

Consequências:
- permite estruturas potencialmente infinitas;
- pode evitar cálculos desnecessários;
- pode criar acúmulo de thunks e consumo de memória se usado sem cuidado.

Estude `seq`, campos estritos (`!`), forma normal (NF) e forma normal fraca de cabeça (WHNF, weak head normal form). Não adicione strictness sem medir ou compreender o efeito.

## 4. Estruturas de dados e desempenho

- Compare listas com `Data.Sequence`, `Data.Vector` e outras estruturas conforme o uso.
- Entenda complexidade temporal e espacial.
- Use profiling para localizar gargalos.
- Meça antes de otimizar.
- Considere o custo de conversões e alocações.

Documentação de bibliotecas: https://hackage.haskell.org/

## 5. Concorrência e paralelismo

Estude progressivamente:
- threads leves e `forkIO`;
- comunicação com `MVar` e `Chan`;
- `async` para composição de tarefas concorrentes;
- paralelismo puro e estratégias, quando adequados;
- cancelamento, exceções e recursos compartilhados.

Concorrência e paralelismo não são sinônimos: concorrência organiza tarefas que progridem de forma independente; paralelismo executa trabalho simultaneamente.

## 6. Efeitos e arquitetura

Em aplicações maiores, explore:
- separar domínio puro de infraestrutura;
- `Reader` para dependências de contexto;
- `State` para cálculos com estado explícito;
- `ExceptT` para erros;
- `ResourceT` e padrões de gerenciamento de recursos, quando aplicáveis.

Bibliotecas de transformers adicionam abstrações úteis, mas não são obrigatórias para programas iniciais.

## 7. Compiladores e análise estática

Haskell é útil para explorar:
- parsers;
- árvores sintáticas abstratas;
- interpretadores;
- análise semântica;
- sistemas de tipos;
- transformações de programas.

Um projeto didático excelente é implementar uma calculadora com parser, AST e avaliador.

## 8. Segurança e confiabilidade

- Prefira tipos que representem estados válidos.
- Evite funções parciais em entradas não confiáveis.
- Use `Either`/`Maybe` para falhas esperadas.
- Teste invariantes.
- Faça profiling e análise de dependências.
- Documente pressupostos e limites.

## 9. Leituras recomendadas

- https://www.haskell.org/documentation/
- https://downloads.haskell.org/ghc/
- https://haskell-language-server.readthedocs.io/
- https://hackage.haskell.org/
- https://wiki.haskell.org/
