/* =====================================================================
 * ATIVIDADE 5 - Vetor que Cresce: realloc na prática
 * ---------------------------------------------------------------------
 * Objetivo: Praticar realloc para criar um vetor que aumenta
 *           dinamicamente conforme o usuário insere dados.
 *
 * Tempo estimado: 30 minutos
 * Compilar: gcc 05_vetor_dinamico.c -o 05_vetor_dinamico -Wall
 *
 * Cenário: o usuário não sabe quantos números vai digitar. O programa
 *          deve aceitar números até ele digitar -1 para parar.
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define TAM_INICIAL 2

int main(void) {
    int capacidade = TAM_INICIAL;
    int qtd = 0;
    int valor;

    /* TODO 1: Aloque um vetor inicial com capacidade TAM_INICIAL.
     *         Verifique se malloc retornou NULL. Se sim, retorne 1.
     */
    int *numeros = NULL;


    printf("Digite numeros (-1 para parar):\n");

    while (1) {
        scanf("%d", &valor);
        if (valor == -1) break;

        /* TODO 2: Se 'qtd' já chegou em 'capacidade', dobre a capacidade
         *         usando realloc.
         *
         *         Padrão SEGURO de realloc:
         *         int *temp = realloc(numeros, nova_capacidade * sizeof(int));
         *         if (temp == NULL) {
         *             // realloc falhou! 'numeros' ainda é válido.
         *             // Libere e saia limpo.
         *             free(numeros);
         *             return 1;
         *         }
         *         numeros = temp;
         *         capacidade = nova_capacidade;
         *
         *         POR QUE usar 'temp'? Se você fizesse:
         *         numeros = realloc(numeros, ...);
         *         e realloc falhasse, você PERDERIA o ponteiro original
         *         e teria um memory leak.
         */


        /* TODO 3: Adicione 'valor' na posição 'qtd' e incremente qtd. */


    }

    /* TODO 4: Imprima quantos números foram lidos e seus valores. */


    /* TODO 5: Calcule e imprima a soma e a média dos números lidos.
     *         Cuidado com divisão por zero se nenhum número foi lido.
     */


    /* TODO 6: LIBERE a memória. */


    return 0;
}
