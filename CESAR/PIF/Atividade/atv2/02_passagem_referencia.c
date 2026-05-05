/* =====================================================================
 * ATIVIDADE 2 - Passagem por Referência: a função troca() clássica
 * ---------------------------------------------------------------------
 * Objetivo: Entender por que precisamos de ponteiros para modificar
 *           variáveis dentro de funções.
 *
 * Tempo estimado: 20 minutos
 * Compilar: gcc 02_passagem_referencia.c -o 02_passagem_referencia -Wall
 * =====================================================================
 */

#include <stdio.h>

/* Função INCORRETA (passagem por valor) - já implementada, NÃO MUDE.
 * Você vai notar que ela NÃO funciona. Por quê?
 */
void troca_errado(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

/* TODO 1: Implemente a função troca_certo() que recebe DOIS PONTEIROS
 *         para int e troca os valores apontados.
 *
 *         Assinatura: void troca_certo(int *a, int *b)
 *         Dica: use uma variável temporária e o operador *.
 */


/* TODO 2: Implemente a função soma_e_diferenca() que recebe dois
 *         inteiros 'a' e 'b' (por valor) e DOIS ponteiros para int
 *         onde deve armazenar a soma e a diferença (a - b).
 *
 *         Assinatura: void soma_e_diferenca(int a, int b,
 *                                            int *soma, int *diff)
 *
 *         Esse padrão (retornar múltiplos valores via ponteiro)
 *         é MUITO usado em C profissional.
 */


int main(void) {
    int x = 10, y = 20;

    printf("Antes: x = %d, y = %d\n", x, y);

    /* Tente trocar com a versão errada */
    troca_errado(x, y);
    printf("Depois de troca_errado: x = %d, y = %d\n", x, y);
    printf(">>> Os valores NÃO trocaram! Por que será?\n\n");

    /* TODO 3: Chame troca_certo() passando OS ENDEREÇOS de x e y.
     *         Imprima os valores depois e confirme que trocaram.
     */


    /* TODO 4: Chame soma_e_diferenca() com os valores 50 e 30.
     *         Armazene os resultados em variáveis locais e imprima.
     *         Saída esperada: soma = 80, diferença = 20.
     */
    int soma, diff;


    return 0;
}
