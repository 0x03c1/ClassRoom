#include <stdio.h>
#include <stdlib.h>  /* malloc, calloc, realloc, free */

int main(void) {
    int n;
    printf("Quantas notas? ");
    scanf("%d", &n);

    /* malloc: aloca n * sizeof(int) bytes no HEAP */
    int *notas = (int *)malloc(n * sizeof(int));

    if (notas == NULL) {   /* SEMPRE verifique! */
        printf("Erro: memória insuficiente.\n");
        return 1;
    }

    /* Usar como array normal */
    for (int i = 0; i < n; i++) {
        printf("Nota %d: ", i + 1);
        scanf("%d", &notas[i]);
    }

    /* Precisamos de mais espaço? realloc! */
    int novo_n = n + 2;
    int *temp = (int *)realloc(notas, novo_n * sizeof(int));
    if (temp != NULL) {
        notas = temp;  /* Só atualiza se realloc teve sucesso */
        notas[n]     = 10;  /* Nova posição */
        notas[n + 1] = 9;
        n = novo_n;
    }

    /* Calcular média */
    int soma = 0;
    for (int i = 0; i < n; i++) soma += notas[i];
    printf("Média: %.1f\n", soma / (float)n);

    /* LIBERAR memória: obrigatório! */
    free(notas);
    notas = NULL;  /* Boa prática: evita dangling pointer */

    return 0;
}
