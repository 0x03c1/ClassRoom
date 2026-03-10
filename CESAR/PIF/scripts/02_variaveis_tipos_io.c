#include <stdio.h>

int main(void) {
    /* Declaração e inicialização */
    int idade = 20;
    float altura = 1.75f;
    char inicial = 'J';

    /* Saída formatada */
    printf("Idade: %d anos\n", idade);
    printf("Altura: %.2f m\n", altura);
    printf("Inicial: %c\n", inicial);

    /* Entrada do usuário */
    printf("\nDigite sua idade: ");
    scanf("%d", &idade);   /* &idade = endereço da variável */

    printf("Você tem %d anos.\n", idade);
    return 0;
}
