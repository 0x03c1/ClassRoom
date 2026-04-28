/*
 * =====================================================================
 * PROBLEMA 01 — SOMA RECURSIVA COM PONTEIRO DE ACUMULADOR
 * Tópicos: recursão, ponteiros, parâmetros por referência
 * =====================================================================
 *
 * Enunciado:
 *   Implemente uma função recursiva que receba um inteiro N (>= 0) e um
 *   ponteiro para um acumulador (long *), e some todos os valores de
 *   1 até N dentro do acumulador apontado.
 *
 *   A função NÃO deve retornar a soma. Em vez disso, deve modificar
 *   o valor apontado pelo ponteiro recebido.
 *
 *   Assinatura obrigatória:
 *       void soma_ate(int n, long *acc);
 *
 * Entrada:
 *   Um inteiro N digitado pelo usuário.
 *
 * Saída:
 *   A soma de 1 + 2 + ... + N.
 *
 * Exemplo:
 *   Entrada: 5
 *   Saída:   Soma de 1 a 5 = 15
 *
 * Restrições:
 *   - Não usar variáveis globais.
 *   - Não usar laços (for/while). A solução DEVE ser recursiva.
 *   - O acumulador deve ser passado por referência (ponteiro).
 */

#include <stdio.h>

/* TODO: Implemente aqui */
void soma_ate(int n, long *acc) {
    /* sua implementação */
}

int main(void) {
    int n;
    long resultado = 0;

    printf("Digite N: ");
    if (scanf("%d", &n) != 1 || n < 0) {
        printf("Entrada invalida.\n");
        return 1;
    }

    soma_ate(n, &resultado);
    printf("Soma de 1 a %d = %ld\n", n, resultado);
    return 0;
}
