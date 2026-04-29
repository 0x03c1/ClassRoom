#include <stdio.h>

void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void inverter(int *inicio, int *fim) {
    if (inicio >= fim) {
        return;
    }

    trocar(inicio, fim);
    inverter(inicio + 1, fim - 1);
}

int main(void) {
    int n, v[25];

    if (scanf("%d", &n) != 1 || n < 1 || n > 25) return 1;

    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }

    inverter(v, v + n - 1);

    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    return 0;
}
