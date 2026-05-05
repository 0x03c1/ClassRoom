/* =====================================================================
 * ATIVIDADE 1 - Ponteiros: Endereço, Valor e Desreferência
 * ---------------------------------------------------------------------
 * Objetivo: Entender a diferença entre o ENDEREÇO de uma variável
 *           e o VALOR armazenado naquele endereço.
 *
 * Tempo estimado: 15 minutos
 * Compilar: gcc 01_ponteiros_basico.c -o 01_ponteiros_basico -Wall
 * =====================================================================
 */

#include <stdio.h>

int main(void) {
    int idade = 21;
    float altura = 1.78f;
    char inicial = 'R';

    /* TODO 1: Declare três ponteiros: p_idade, p_altura, p_inicial.
     *         Cada um deve apontar para a variável correspondente.
     *         Exemplo: int *p_idade = &idade;
     */


    /* TODO 2: Imprima o ENDEREÇO de cada variável usando o operador &.
     *         Use o especificador %p e converta para (void*).
     *         Exemplo: printf("Endereço de idade: %p\n", (void*)&idade);
     */


    /* TODO 3: Imprima o VALOR de cada variável de DUAS formas:
     *         (a) acessando direto: idade, altura, inicial
     *         (b) desreferenciando o ponteiro: *p_idade, *p_altura, *p_inicial
     *         Os valores devem ser idênticos.
     */


    /* TODO 4: Modifique o valor de 'idade' através do ponteiro p_idade
     *         (use *p_idade = 30;) e imprima 'idade' depois para confirmar
     *         que a variável original foi alterada.
     */


    return 0;
}
