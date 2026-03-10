#include <stdio.h>

int soma_quadrados_pares(int arr[], int n) {
    int soma = 0;                    /* Estado mutável */
    for (int i = 0; i < n; i++) {    /* Loop explícito */
        if (arr[i] % 2 == 0) {       /* Decisão explícita */
            soma += arr[i] * arr[i]; /* Modificação de estado */
        }
    }
    return soma;
}

int main(void) {
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("Resultado: %d\n", soma_quadrados_pares(nums, 10));
    /* Resultado: 4 + 16 + 36 + 64 + 100 = 220 */
    return 0;
}
