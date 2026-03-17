#include <stdio.h>

int main(void) {
    /* Array unidimensional */
    int notas[5] = {8, 7, 9, 6, 10};
    int soma = 0;

    for (int i = 0; i < 5; i++) {
        soma += notas[i];   /* Acesso por índice (começa em 0!) */
    }
    printf("Média: %.1f\n", soma / 5.0);

    /* Matriz 3x3 (array bidimensional) */
    int matriz[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    printf("\nMatriz:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%3d ", matriz[i][j]);
        }
        printf("\n");
    }

    /* Diagonal principal: i == j */
    printf("\nDiagonal principal: ");
    for (int i = 0; i < 3; i++) {
        printf("%d ", matriz[i][i]);
    }
    printf("\n");

    return 0;
}
