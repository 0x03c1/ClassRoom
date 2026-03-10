#include <stdio.h>

int main(void) {
    float nota;
    printf("Digite a nota (0-10): ");
    scanf("%f", &nota);

    if (nota >= 7.0f) {
        printf("Aprovado! Parabéns.\n");
    } else if (nota >= 5.0f) {
        printf("Recuperação. Estude mais.\n");
    } else {
        printf("Reprovado. Não desista!\n");
    }

    return 0;
}
