# Lista de exercícios

Tente resolver os exercícios antes de consultar soluções externas. Para cada função, escreva uma assinatura de tipo e teste casos comuns e extremos.

## Nível 1 — Expressões e funções

1. Calcule `3 + 4 * 2` e explique a precedência.
2. Escreva `dobro :: Int -> Int`.
3. Escreva `areaCirculo :: Double -> Double`.
4. Escreva uma função que converta minutos em segundos.
5. Escreva `ehPar :: Int -> Bool`.

## Nível 2 — Condições e recursão

6. Classifique um inteiro como negativo, zero ou positivo.
7. Implemente fatorial para inteiros não negativos.
8. Implemente a soma de uma lista recursivamente.
9. Conte quantos elementos pares existem numa lista.
10. Inverta uma lista sem chamar `reverse`.

## Nível 3 — Ordem superior

11. Use `map` para elevar números ao quadrado.
12. Use `filter` para selecionar números primos de uma lista pequena.
13. Use `foldr` para concatenar uma lista de strings.
14. Defina uma função que aplique outra função duas vezes.
15. Use composição para transformar e formatar dados.

## Nível 4 — Tipos e erros

16. Modele um semáforo com `data`.
17. Escreva uma função que retorne `Maybe` para o primeiro elemento de uma lista.
18. Escreva divisão segura usando `Either`.
19. Modele uma tarefa com identificador, descrição e status.
20. Valide uma string que deve conter pelo menos um caractere não branco.

## Nível 5 — Abstrações e engenharia

21. Use `fmap` sobre `Maybe`.
22. Combine dois valores `Maybe` com Applicative.
23. Encadeie operações que podem falhar.
24. Escreva testes unitários para funções de listas.
25. Crie um projeto Cabal com biblioteca, executável e testes.

## Desafio integrador

Implemente um analisador de notas acadêmicas que:
- leia notas de um arquivo;
- valide os registros;
- calcule média e estatísticas;
- classifique resultados usando tipos próprios;
- registre entradas inválidas sem encerrar todo o processamento;
- inclua testes e documentação.

## Autoavaliação

Para cada exercício, responda:
- O tipo da função expressa bem a intenção?
- O que acontece com entrada vazia ou inválida?
- Existem funções parciais que podem falhar?
- É possível decompor a solução em funções puras menores?
- Os casos de teste cobrem limites relevantes?
