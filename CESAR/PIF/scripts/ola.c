#include <stdio.h>

int main() {
    char nome[50];
    int idade;
    float media;

    printf("Digite seu nome: ");
    scanf("%s", nome);
    printf("Digite sua idade: ");
    scanf("%d", &idade);
    printf("Digite sua media: ");
    scanf("%f", &media);

    printf("Nome: %s\n", nome);
    printf("Idade: %d anos\n", idade);
    printf("Media: %.2f\n", media);

    return 0;
}
