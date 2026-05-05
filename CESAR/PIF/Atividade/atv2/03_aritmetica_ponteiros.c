/* =====================================================================
 * ATIVIDADE 3 - Aritmética de Ponteiros: Vetor e Ponteiro
 * ---------------------------------------------------------------------
 * Objetivo: Descobrir que vetor e ponteiro são "primos próximos" em C.
 *           Praticar percorrer um vetor usando ponteiros.
 *
 * Tempo estimado: 25 minutos
 * Compilar: gcc 03_aritmetica_ponteiros.c -o 03_aritmetica_ponteiros -Wall
 * =====================================================================
 */

#include <stdio.h>

#define TAM 5

/* TODO 1: Implemente a função imprime_vetor_indice() que recebe um
 *         vetor de int e seu tamanho, e imprime usando NOTAÇÃO DE ÍNDICE
 *         (vet[i]).
 *
 *         Assinatura: void imprime_vetor_indice(int vet[], int tam)
 */


/* TODO 2: Implemente a função imprime_vetor_ponteiro() que faz o MESMO,
 *         mas usando NOTAÇÃO DE PONTEIRO: *(vet + i) ou ainda melhor,
 *         um ponteiro local que avança com vet++.
 *
 *         Assinatura: void imprime_vetor_ponteiro(int *vet, int tam)
 */


/* TODO 3: Implemente a função soma_vetor() que retorna a soma de
 *         todos os elementos. Use APENAS aritmética de ponteiros
 *         (não use colchetes [ ]).
 *
 *         Assinatura: int soma_vetor(int *vet, int tam)
 *         Dica: use um ponteiro auxiliar que vai de vet até vet+tam-1.
 */


/* TODO 4: Implemente busca_maior() que retorna o MAIOR valor do vetor.
 *         Use ponteiros, não índices.
 *
 *         Assinatura: int busca_maior(int *vet, int tam)
 */


int main(void) {
    int numeros[TAM] = {15, 42, 7, 23, 99};

    printf("=== Notação de índice ===\n");
    /* TODO 5: Chame imprime_vetor_indice() */


    printf("\n=== Notação de ponteiro ===\n");
    /* TODO 6: Chame imprime_vetor_ponteiro() */


    /* TODO 7: Chame soma_vetor() e imprima o resultado.
     *         Resposta esperada: 186
     */


    /* TODO 8: Chame busca_maior() e imprima o resultado.
     *         Resposta esperada: 99
     */


    /* DESAFIO EXTRA (opcional):
     * O que essas duas linhas imprimem? Tente prever ANTES de rodar.
     *
     *   printf("%d\n", numeros[2]);
     *   printf("%d\n", *(numeros + 2));
     *
     * São equivalentes? Por quê?
     */

    return 0;
}
