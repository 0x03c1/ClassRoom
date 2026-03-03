#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;

    printf("Quantos elementos? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Valor invalido!\n");
        return 1;
    }

    int *v = (int *) malloc(n * sizeof(int));
    if (v == NULL) {
        printf("Erro de alocacao!\n");
        return 1;
    }

    // Entrada de dados
    for (int i = 0; i < n; i++) {
        printf("v[%d] = ", i);
        if (scanf("%d", &v[i]) != 1) {
            printf("Entrada invalida!\n");
            free(v);
            return 1;
        }
    }

    // --- Processamento ---
    int soma = 0;
    int maior = v[0];
    int menor = v[0];

    for (int i = 0; i < n; i++) {
        soma += v[i];

        if (v[i] > maior) {
            maior = v[i];
        }

        if (v[i] < menor) {
            menor = v[i];
        }
    }

    float media = (float) soma / n;

    // --- Saída ---
    printf("\nResultados:\n");
    printf("Soma = %d\n", soma);
    printf("Media = %.2f\n", media);
    printf("Maior = %d\n", maior);
    printf("Menor = %d\n", menor);

    // Liberação da memória
    free(v);

    return 0;
}
