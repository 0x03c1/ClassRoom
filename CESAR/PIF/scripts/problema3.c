#include <stdio.h>

int pares_vezes_dois(int lista[], int tamanho, int resultado[]) {
    int count = 0;

    for (int i = 0; i < tamanho; i++) {
        if (lista[i] % 2 == 0) {              // é par?
            resultado[count] = lista[i] * 2;   // guarda o dobro
            count++;
        }
    }

    return count;   // quantos pares foram encontrados
}

int main() {
    int lista[] = {3, 8, 15, 22, 7, 10, 1, 4};
    int tamanho = sizeof(lista) / sizeof(lista[0]);

    printf("========================================\n");
    printf("  Filtrar Pares e Multiplicar por 2\n");
    printf("========================================\n");

    // Mostra a lista original
    printf("Lista original: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", lista[i]);
    }
    printf("\n");

    // Aplica o filtro
    int resultado[tamanho];
    int qtd = pares_vezes_dois(lista, tamanho, resultado);

    // Mostra o resultado
    printf("Pares x 2:      ");
    for (int i = 0; i < qtd; i++) {
        printf("%d ", resultado[i]);
    }
    printf("\n");

    return 0;
}
