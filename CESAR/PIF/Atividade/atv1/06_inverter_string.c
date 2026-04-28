/*
 * =====================================================================
 * PROBLEMA 06 — INVERTER STRING IN-PLACE RECURSIVAMENTE
 * Tópicos: recursão, ponteiros, strings em C, manipulação de char *
 * =====================================================================
 *
 * Enunciado:
 *   Implemente uma função recursiva que inverta uma string IN-PLACE
 *   (sem alocar memória adicional).
 *
 *   Assinatura obrigatória:
 *       void inverter_str(char *inicio, char *fim);
 *
 *   Antes de chamá-la, você deve calcular o ponteiro para o último
 *   caractere usando uma função AUXILIAR RECURSIVA que ache o '\0' final
 *   sem usar strlen() nem laços:
 *       char *fim_da_string(char *s);
 *
 *   Restrições:
 *     - Proibido usar strlen().
 *     - Proibido usar laços (for/while).
 *     - Proibido usar índice s[i].
 *
 * Entrada:
 *   Uma string de até 199 caracteres.
 *
 * Saída:
 *   A string invertida.
 *
 * Exemplo:
 *   Entrada: recursao
 *   Saída:   oasrucer
 */

#include <stdio.h>

/* TODO: encontre o '\0' final recursivamente e devolva o ponteiro
 *       para o último caractere (ou para o próprio '\0' se a string
 *       estiver vazia). */
char *fim_da_string(char *s) {
    return s;
}

/* TODO */
void inverter_str(char *inicio, char *fim) {
    /* sua implementação */
}

int main(void) {
    char buf[200];
    printf("String: ");
    if (!fgets(buf, sizeof(buf), stdin)) return 1;

    char *p = buf;
    while (*p && *p != '\n') p++;
    *p = '\0';

    if (buf[0] == '\0') { printf("\n"); return 0; }

    char *fim = fim_da_string(buf);
    /* fim_da_string deve retornar o último caractere, não o '\0'.
       Caso retorne o '\0', recue um. Aqui assumimos que devolve o
       último caractere válido. */
    inverter_str(buf, fim);

    printf("Invertido: %s\n", buf);
    return 0;
}
