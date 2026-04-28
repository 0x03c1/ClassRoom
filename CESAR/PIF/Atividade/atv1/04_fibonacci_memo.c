/*
 * =====================================================================
 * PROBLEMA 04 — FIBONACCI RECURSIVO COM MEMOIZATION
 * Tópicos: recursão, ponteiros para vetor, programação dinâmica
 * =====================================================================
 *
 * Enunciado:
 *   Implemente uma função recursiva que calcule F(n) — o n-ésimo
 *   número de Fibonacci — utilizando MEMOIZATION através de um
 *   vetor de cache passado por PONTEIRO.
 *
 *   Definição: F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2).
 *
 *   Assinatura obrigatória:
 *       long fib(int n, long *cache);
 *
 *   O cache deve ser inicializado com -1 em todas as posições para
 *   indicar "não calculado". Quando a função encontrar cache[n] != -1,
 *   deve devolver o valor armazenado em vez de recalcular.
 *
 *   Restrições:
 *     - Cache alocado dinamicamente com malloc(), liberado com free().
 *     - Acesso ao cache exclusivamente via ponteiro (não use vetor
 *       global).
 *
 * Entrada:
 *   Um inteiro N (0 <= N <= 90).
 *
 * Saída:
 *   F(N).
 *
 * Exemplo:
 *   Entrada: 10
 *   Saída:   F(10) = 55
 *
 *   Entrada: 50
 *   Saída:   F(50) = 12586269025
 */

#include <stdio.h>
#include <stdlib.h>

/* TODO */
long fib(int n, long *cache) {
    return 0;
}

int main(void) {
    int n;
    printf("N: ");
    if (scanf("%d", &n) != 1 || n < 0 || n > 90) {
        printf("Entrada invalida.\n");
        return 1;
    }

    long *cache = malloc((n + 1) * sizeof(long));
    if (!cache) return 1;
    for (int i = 0; i <= n; i++) cache[i] = -1;

    printf("F(%d) = %ld\n", n, fib(n, cache));

    free(cache);
    return 0;
}
