/*
 * =====================================================================
 * PROBLEMA 10 — TORRE DE HANÓI COM PILHAS DE DISCOS (PONTEIROS)
 * Tópicos: recursão clássica, ponteiros para struct, simulação de
 *          pilhas em vetores, validação de movimentos
 * =====================================================================
 *
 * Enunciado:
 *   Resolva o problema clássico da Torre de Hanói para N discos, mas
 *   com um requisito adicional: as três torres devem ser representadas
 *   como pilhas REAIS (struct Pilha) e cada movimento deve ser
 *   EXECUTADO de fato (transferindo o disco entre pilhas) e VALIDADO
 *   (não pode colocar disco maior sobre disco menor).
 *
 *   Estrutura obrigatória:
 *
 *     #define MAX 32
 *     typedef struct {
 *         int dados[MAX];   /* discos representados pelo seu tamanho */
 *         int topo;         /* -1 = pilha vazia                       */
 *         char nome;        /* 'A', 'B' ou 'C'                        */
 *     } Pilha;
 *
 *   Você deve implementar:
 *
 *     void empilhar(Pilha *p, int disco);
 *     int  desempilhar(Pilha *p);
 *     int  topo(const Pilha *p);   /* retorna -1 se vazia */
 *
 *     void mover(Pilha *origem, Pilha *destino);
 *         /* Desempilha de 'origem', verifica se cabe em 'destino'
 *            (topo(destino) == -1 ou disco < topo(destino)) e
 *            empilha. Em caso de movimento inválido, aborta com
 *            mensagem de erro. */
 *
 *     void hanoi(int n, Pilha *origem, Pilha *destino, Pilha *aux);
 *         /* Algoritmo recursivo clássico. */
 *
 *   Comportamento esperado:
 *     - Inicialize a pilha A com discos N, N-1, ..., 1 do fundo para o
 *       topo (o disco 1 é o menor e fica no topo).
 *     - Imprima cada movimento no formato "Mover disco X de A para C".
 *     - Imprima o número total de movimentos ao final (deve ser
 *       2^N - 1).
 *
 * Entrada:
 *   N (1 <= N <= 10).
 *
 * Saída:
 *   Sequência de movimentos e total.
 *
 * Exemplo:
 *   Entrada: 3
 *   Saída:
 *     Mover disco 1 de A para C
 *     Mover disco 2 de A para B
 *     Mover disco 1 de C para B
 *     Mover disco 3 de A para C
 *     Mover disco 1 de B para A
 *     Mover disco 2 de B para C
 *     Mover disco 1 de A para C
 *     Total de movimentos: 7
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX 32

typedef struct {
    int dados[MAX];
    int topo;
    char nome;
} Pilha;

static int total_movimentos = 0;

/* TODO */
void empilhar(Pilha *p, int disco) {
}

/* TODO */
int desempilhar(Pilha *p) {
    return -1;
}

/* TODO */
int topo(const Pilha *p) {
    return -1;
}

/* TODO */
void mover(Pilha *origem, Pilha *destino) {
}

/* TODO */
void hanoi(int n, Pilha *origem, Pilha *destino, Pilha *aux) {
}

int main(void) {
    int n;
    printf("N (1..10): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 10) return 1;

    Pilha A = { .topo = -1, .nome = 'A' };
    Pilha B = { .topo = -1, .nome = 'B' };
    Pilha C = { .topo = -1, .nome = 'C' };

    /* preenche A: do maior (n) para o menor (1) */
    for (int d = n; d >= 1; d--) empilhar(&A, d);

    hanoi(n, &A, &C, &B);

    printf("Total de movimentos: %d\n", total_movimentos);
    return 0;
}
