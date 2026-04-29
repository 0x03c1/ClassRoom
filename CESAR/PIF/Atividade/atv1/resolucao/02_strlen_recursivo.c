/*
 * GRUPO===============================================================
 * - Lucas Gabriel
 * - Igor Aragão
 * - Breno Gabriel
 * - André Montenegro
 * =====================================================================
 * PROBLEMA 02 — TAMANHO DE STRING RECURSIVO
 * Tópicos: recursão, aritmética de ponteiros, strings em C
 * =====================================================================
 *
 * Enunciado:
 *   Implemente uma função recursiva que calcule o comprimento de uma
 *   string (sem contar o '\0' final) USANDO ARITMÉTICA DE PONTEIROS,
 *   sem indexação por colchetes [].
 *
 *   Assinatura obrigatória:
 *       int meu_strlen(const char *s);
 *
 *   Restrições:
 *     - Não usar a função strlen() da <string.h>.
 *     - Não usar laços (for/while).
 *     - Não usar a sintaxe s[i].
 *     - Acesse os caracteres exclusivamente via *s e (s+1).
 *
 * Entrada:
 *   Uma linha de texto digitada pelo usuário (até 199 caracteres).
 *
 * Saída:
 *   O número de caracteres da string.
 *
 * Exemplo:
 *   Entrada: ola mundo
 *   Saída:   Tamanho = 9
 */

#include <stdio.h>

/* TODO: Implemente aqui */
int meu_strlen(const char *s) {
    
    if (*s == '\0') {
        return 0;
    };
    
    return 1 + meu_strlen(s+1);
};

int main(void) {
    char buffer[200];
    printf("Digite uma string: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return 1;

    /* remove '\n' do final, se houver */
    char *p = buffer;
    while (*p && *p != '\n') p++;
    *p = '\0';

    printf("Tamanho = %d\n", meu_strlen(buffer));
    return 0;
}
