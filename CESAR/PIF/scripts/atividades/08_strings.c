#include <stdio.h>
#include <string.h>  /* Funções para manipulação de strings */

int main(void) {
    /* Declarações equivalentes */
    char nome1[] = "Maria";            /* Compilador calcula: 6 bytes (5+\0) */
    char nome2[20];                     /* Buffer de 20 bytes */

    /* Leitura segura de strings */
    printf("Digite seu nome: ");
    fgets(nome2, sizeof(nome2), stdin); /* Lê até 19 chars + \0 */

    /* Remove o \n que fgets captura */
    nome2[strcspn(nome2, "\n")] = '\0';

    /* Funções essenciais de <string.h> */
    printf("Comprimento de '%s': %lu\n", nome1, strlen(nome1));

    char destino[50];
    strcpy(destino, nome1);             /* Copia nome1 para destino */
    strcat(destino, " e ");             /* Concatena */
    strcat(destino, nome2);

    printf("Resultado: %s\n", destino);

    /* Comparação (NÃO use == para strings!) */
    if (strcmp(nome1, nome2) == 0) {
        printf("Nomes iguais!\n");
    } else {
        printf("Nomes diferentes.\n");
    }

    return 0;
}
