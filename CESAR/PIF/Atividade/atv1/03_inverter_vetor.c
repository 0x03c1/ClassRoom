/*
 * =====================================================================
 * PROBLEMA 03 — INVERTER VETOR IN-PLACE COM DOIS PONTEIROS
 * Tópicos: recursão, ponteiros, vetores, troca via ponteiro
 * =====================================================================
 *
 * Enunciado:
 *   Implemente uma função recursiva que inverta um vetor de inteiros
 *   IN-PLACE (sem usar vetor auxiliar) usando dois ponteiros: um para
 *   o início e outro para o fim do trecho a ser invertido.
 *
 *   Assinatura obrigatória:
 *       void inverter(int *inicio, int *fim);
 *
 *   Você também deve implementar uma função auxiliar de troca:
 *       void trocar(int *a, int *b);
 *
 *   Restrições:
 *     - Não usar laços (for/while) na função inverter().
 *     - Não criar vetor auxiliar.
 *     - Use SOMENTE aritmética de ponteiros (não índices).
 *
 * Entrada:
 *   Primeiro um inteiro N (1 <= N <= 100), depois N inteiros.
 *
 * Saída:
 *   O vetor invertido.
 *
 * Exemplo:
 *   Entrada: 5
 *            1 2 3 4 5
 *   Saída:   5 4 3 2 1
 */

#include <stdio.h>

/* TODO */
void trocar(int *a, int *b) {
    /* sua implementação */
}

/* TODO */
void inverter(int *inicio, int *fim) {
    /* sua implementação */
}

int main(void) {
    int n, v[100];
    printf("N: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 100) return 1;

    printf("Elementos: ");
    for (int i = 0; i < n; i++) scanf("%d", &v[i]);

    inverter(v, v + n - 1);

    printf("Invertido: ");
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
    return 0;
}
