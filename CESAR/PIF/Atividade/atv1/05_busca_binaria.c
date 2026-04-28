/*
 * =====================================================================
 * PROBLEMA 05 — BUSCA BINÁRIA RECURSIVA COM PONTEIROS
 * Tópicos: recursão, divisão e conquista, ponteiros, vetores ordenados
 * =====================================================================
 *
 * Enunciado:
 *   Dado um vetor de inteiros JÁ ORDENADO em ordem crescente, implemente
 *   uma função recursiva de busca binária que retorne o PONTEIRO para o
 *   elemento procurado, ou NULL se ele não estiver presente.
 *
 *   Assinatura obrigatória:
 *       int *busca(int *inicio, int *fim, int alvo);
 *
 *   Onde:
 *     - inicio aponta para o primeiro elemento do trecho de busca;
 *     - fim    aponta para o ÚLTIMO elemento (inclusivo) do trecho.
 *
 *   Restrições:
 *     - Não usar laços.
 *     - Use aritmética de ponteiros para calcular o "meio":
 *           meio = inicio + (fim - inicio) / 2;
 *
 * Entrada:
 *   N (1 <= N <= 100), N inteiros ordenados, e o valor a buscar.
 *
 * Saída:
 *   Posição (índice 0-based) do elemento, ou "Nao encontrado".
 *
 * Exemplo:
 *   Entrada: 6
 *            1 3 5 7 9 11
 *            7
 *   Saída:   Encontrado no indice 3
 */

#include <stdio.h>
#include <stddef.h>

/* TODO */
int *busca(int *inicio, int *fim, int alvo) {
    return NULL;
}

int main(void) {
    int n, v[100], alvo;
    printf("N: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 100) return 1;

    printf("Elementos (ordenados): ");
    for (int i = 0; i < n; i++) scanf("%d", &v[i]);

    printf("Alvo: ");
    scanf("%d", &alvo);

    int *p = busca(v, v + n - 1, alvo);
    if (p) printf("Encontrado no indice %ld\n", (long)(p - v));
    else   printf("Nao encontrado\n");

    return 0;
}
