/*
 * =====================================================================
 * PROBLEMA 09 — QUICKSORT COM PONTEIROS
 * Tópicos: recursão, divisão e conquista, ponteiros, partição in-place
 * =====================================================================
 *
 * Enunciado:
 *   Implemente o algoritmo Quicksort em C usando exclusivamente
 *   PONTEIROS (sem índices) para particionar e ordenar um vetor de
 *   inteiros em ordem CRESCENTE.
 *
 *   Você deve implementar:
 *
 *     void trocar(int *a, int *b);
 *
 *     int *particionar(int *inicio, int *fim);
 *         /* Particiona o trecho [inicio..fim] usando o pivô = *fim
 *            (esquema de Lomuto). Devolve o ponteiro para a posição
 *            final do pivô. */
 *
 *     void quicksort(int *inicio, int *fim);
 *         /* Caso base: inicio >= fim. Caso recursivo: particiona e
 *            ordena os dois lados. */
 *
 *   Restrições:
 *     - Proibido usar índices (v[i]) dentro de particionar/quicksort.
 *     - Proibido qsort() da <stdlib.h>.
 *     - Use a aritmética de ponteiros para percorrer o trecho.
 *
 * Entrada:
 *   N (1 <= N <= 1000), seguido de N inteiros.
 *
 * Saída:
 *   Vetor ordenado.
 *
 * Exemplo:
 *   Entrada: 6
 *            5 2 8 1 9 3
 *   Saída:   1 2 3 5 8 9
 */

#include <stdio.h>

/* TODO */
void trocar(int *a, int *b) {
}

/* TODO */
int *particionar(int *inicio, int *fim) {
    return inicio;
}

/* TODO */
void quicksort(int *inicio, int *fim) {
}

int main(void) {
    int n, v[1000];
    printf("N: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 1000) return 1;

    printf("Elementos: ");
    for (int i = 0; i < n; i++) scanf("%d", &v[i]);

    quicksort(v, v + n - 1);

    printf("Ordenado: ");
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
    return 0;
}
