#include <stdio.h>

int main(void) {
    int opcao;
    printf("Menu: 1-Novo  2-Abrir  3-Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Criando novo arquivo...\n");
            break;     /* Sem break, executa o próximo case! */
        case 2:
            printf("Abrindo arquivo...\n");
            break;
        case 3:
            printf("Saindo. Até logo!\n");
            break;
        default:
            printf("Opção inválida.\n");
    }

    return 0;
}
