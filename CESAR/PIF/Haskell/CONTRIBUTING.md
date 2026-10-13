# Como contribuir

Obrigado por ajudar a melhorar este roteiro de Haskell.

## Diretrizes

- Escreva em português claro e mantenha os termos técnicos em inglês quando forem nomes consagrados da linguagem.
- Prefira exemplos pequenos, completos e executáveis.
- Indique a versão ou as condições necessárias quando um exemplo depender de ferramenta, pacote ou extensão.
- Não apresente como parte do Haskell padrão algo que dependa de uma extensão do GHC sem explicar isso.
- Informe a saída esperada quando isso ajudar o estudante.
- Evite exemplos que exijam serviços externos sem oferecer uma alternativa local.

## Estrutura sugerida para um módulo

1. Objetivos de aprendizagem.
2. Conceitos e motivação.
3. Exemplos executáveis.
4. Erros comuns.
5. Exercícios.
6. Leituras recomendadas.

## Verificação local

Antes de enviar uma contribuição:

```bash
ghc --version
cabal --version
```

Execute os exemplos `.hs` que forem modificados. Se um exemplo depender de um pacote, documente como instalá-lo ou executá-lo por meio do Cabal.

## Pull requests

Explique o problema resolvido, resuma as alterações e indique como verificou os exemplos.
