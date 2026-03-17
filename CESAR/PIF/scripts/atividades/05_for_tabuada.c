#include <stdio.h>

int main(void) {
    /* Tabuada de um número */
    int num;
    printf("Digite um número para ver a tabuada: ");
    scanf("%d", &num);

    for (int i = 1; i <= 10; i++) {
        printf("%d x %d = %d\n", num, i, num * i);
    }
    /*   init;  condição;  incremento
     *   ^^^^   ^^^^^^^^   ^^^^^^^^^
     *   Executa  Testa     Executa após
     *   1 vez    antes     cada iteração
     */
    return 0;
}
