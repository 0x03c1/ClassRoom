/* =====================================================================
 * ATIVIDADE 4 - Memória Dinâmica: malloc, free e o Heap
 * ---------------------------------------------------------------------
 * Objetivo: Alocar memória em tempo de execução, USAR e LIBERAR.
 *           Aprender o ciclo: malloc -> verificar -> usar -> free.
 *
 * Tempo estimado: 30 minutos
 * Compilar: gcc 04_alocacao_dinamica.c -o 04_alocacao_dinamica -Wall
 *
 * BÔNUS (se valgrind disponível):
 *   valgrind --leak-check=full ./04_alocacao_dinamica
 *   >>> Objetivo: ZERO LEAKS!
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/* TODO 1: Implemente cria_vetor() que recebe um tamanho 'n' e:
 *         (a) aloca um vetor de n inteiros usando malloc
 *         (b) verifica se a alocação deu certo (NULL?)
 *         (c) se falhou, imprime erro e retorna NULL
 *         (d) se deu certo, retorna o ponteiro
 *
 *         Assinatura: int* cria_vetor(int n)
 */


/* TODO 2: Implemente preenche_vetor() que recebe um ponteiro e tamanho
 *         e pede ao usuário n inteiros (use scanf).
 *
 *         Assinatura: void preenche_vetor(int *vet, int n)
 */


/* TODO 3: Implemente calcula_media() que retorna a média (float) dos
 *         elementos do vetor.
 *
 *         Assinatura: float calcula_media(int *vet, int n)
 *         Cuidado: divisão de inteiros em C TRUNCA. Use cast para float.
 */


int main(void) {
    int n;

    printf("Quantos numeros voce quer armazenar? ");
    scanf("%d", &n);

    /* TODO 4: Valide a entrada. Se n <= 0, imprima erro e retorne 1. */


    /* TODO 5: Chame cria_vetor(n) e armazene em uma variável 'numeros'.
     *         Se retornou NULL, retorne 1.
     */


    /* TODO 6: Chame preenche_vetor() para popular o vetor. */


    /* TODO 7: Chame calcula_media() e imprima o resultado com 2 casas. */


    /* TODO 8: LIBERE a memória com free() e atribua NULL ao ponteiro
     *         (defesa contra "use-after-free").
     *
     *         free(numeros);
     *         numeros = NULL;
     */


    return 0;
}
