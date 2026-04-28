/*
 * =====================================================================
 * PROBLEMA 07 — CONTAR OCORRÊNCIAS DE SUBSTRING (RECURSIVO)
 * Tópicos: recursão, ponteiros, casamento de padrões em strings
 * =====================================================================
 *
 * Enunciado:
 *   Implemente uma função recursiva que conte quantas vezes uma
 *   substring (padrao) aparece dentro de uma string (texto). Devem ser
 *   contadas ocorrências SOBREPOSTAS.
 *
 *   Exemplo de sobreposição:
 *     texto  = "ababa"
 *     padrao = "aba"
 *     -> ocorrências em índices 0 e 2 -> total = 2.
 *
 *   Você deve implementar duas funções recursivas auxiliares:
 *
 *     int comeca_com(const char *texto, const char *padrao);
 *         /* devolve 1 se 'texto' começa exatamente com 'padrao',
 *            e 0 caso contrário. NÃO use strncmp(); compare caractere
 *            a caractere recursivamente. */
 *
 *     int contar(const char *texto, const char *padrao);
 *         /* devolve o número de ocorrências do padrão em texto. */
 *
 *   Restrições:
 *     - Não usar strstr(), strncmp(), strcmp().
 *     - Não usar laços (for/while).
 *
 * Entrada:
 *   Duas linhas: o texto e o padrão.
 *
 * Saída:
 *   Número de ocorrências.
 *
 * Exemplo:
 *   Entrada: ababababa
 *            aba
 *   Saída:   Ocorrencias: 4
 */

#include <stdio.h>

/* TODO */
int comeca_com(const char *texto, const char *padrao) {
    return 0;
}

/* TODO */
int contar(const char *texto, const char *padrao) {
    return 0;
}

static void chomp(char *s) {
    while (*s && *s != '\n') s++;
    *s = '\0';
}

int main(void) {
    char texto[300], padrao[100];
    printf("Texto: ");
    if (!fgets(texto, sizeof(texto), stdin)) return 1;
    chomp(texto);

    printf("Padrao: ");
    if (!fgets(padrao, sizeof(padrao), stdin)) return 1;
    chomp(padrao);

    if (padrao[0] == '\0') { printf("Padrao vazio.\n"); return 1; }

    printf("Ocorrencias: %d\n", contar(texto, padrao));
    return 0;
}
