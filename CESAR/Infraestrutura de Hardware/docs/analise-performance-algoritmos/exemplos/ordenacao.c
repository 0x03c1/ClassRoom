/*
 * Exemplo de algoritmo para o comparador_stacks.py
 * Mesma operação que ordenacao.py, mas em C — para comparação direta.
 *
 * Usa pivô por mediana-de-três para reduzir o pior caso do quicksort
 * (mantém comparabilidade entre as 3 linguagens).
 *
 * Compilar:  gcc -O2 ordenacao.c -o ordenacao_c
 * Uso:       ./ordenacao_c <tamanho>
 */

#include <stdio.h>
#include <stdlib.h>

static inline void troca(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

void quicksort(int *arr, int lo, int hi) {
    if (lo < hi) {
        /* Mediana-de-três (lo, mid, hi) — coloca o pivô em arr[hi] */
        int mid = lo + (hi - lo) / 2;
        if (arr[lo] > arr[mid]) troca(&arr[lo], &arr[mid]);
        if (arr[lo] > arr[hi])  troca(&arr[lo], &arr[hi]);
        if (arr[mid] > arr[hi]) troca(&arr[mid], &arr[hi]);
        troca(&arr[mid], &arr[hi]);

        int pivot = arr[hi];
        int i = lo - 1;
        for (int j = lo; j < hi; j++) {
            if (arr[j] <= pivot) {
                i++;
                troca(&arr[i], &arr[j]);
            }
        }
        troca(&arr[i + 1], &arr[hi]);
        int p = i + 1;
        quicksort(arr, lo, p - 1);
        quicksort(arr, p + 1, hi);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <tamanho>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    int *arr = malloc(sizeof(int) * (size_t)n);
    if (!arr) {
        fprintf(stderr, "Falha de alocação\n");
        return 1;
    }
    srand(42);
    for (int i = 0; i < n; i++) arr[i] = rand() % 1000001;

    quicksort(arr, 0, n - 1);

    printf("%d ... %d\n", arr[0], arr[n - 1]);
    free(arr);
    return 0;
}
