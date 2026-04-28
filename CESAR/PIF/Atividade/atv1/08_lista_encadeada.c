/*
 * =====================================================================
 * PROBLEMA 08 — LISTA ENCADEADA COM OPERAÇÕES RECURSIVAS
 * Tópicos: ponteiros, struct, alocação dinâmica, recursão sobre listas,
 *          ponteiro para ponteiro
 * =====================================================================
 *
 * Enunciado:
 *   Implemente uma lista simplesmente encadeada de inteiros com as
 *   seguintes operações, TODAS recursivas:
 *
 *     1) inserir_fim(No **cabeca, int v)
 *           Insere um novo nó com valor v ao FINAL da lista.
 *           Use ponteiro para ponteiro (No **) para tratar o caso da
 *           lista vazia sem variáveis globais nem retornos.
 *
 *     2) imprimir(No *p)
 *           Imprime os valores na ordem natural, separados por " -> ".
 *           Ao final, imprime "NULL\n".
 *
 *     3) imprimir_inversa(No *p)
 *           Imprime os valores na ORDEM INVERSA usando o desempilhamento
 *           da pilha de recursão (sem inverter a lista, sem usar vetor).
 *
 *     4) liberar(No **cabeca)
 *           Libera todos os nós com free() recursivamente e zera *cabeca.
 *
 *   Estrutura obrigatória:
 *     typedef struct No {
 *         int valor;
 *         struct No *prox;
 *     } No;
 *
 *   Restrições:
 *     - Todas as quatro funções devem ser recursivas (sem laços).
 *     - inserir_fim e liberar precisam usar No **.
 *
 * Entrada:
 *   N (número de elementos), seguido de N inteiros.
 *
 * Saída:
 *   Lista direta, lista inversa.
 *
 * Exemplo:
 *   Entrada: 4
 *            10 20 30 40
 *   Saída:   Lista: 10 -> 20 -> 30 -> 40 -> NULL
 *            Inversa: 40 -> 30 -> 20 -> 10 -> NULL
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *prox;
} No;

/* TODO */
void inserir_fim(No **cabeca, int v) {
    /* sua implementação */
}

/* TODO */
void imprimir(No *p) {
    /* sua implementação */
}

/* TODO */
void imprimir_inversa(No *p) {
    /* sua implementação */
}

/* TODO */
void liberar(No **cabeca) {
    /* sua implementação */
}

int main(void) {
    int n;
    No *lista = NULL;

    printf("N: ");
    if (scanf("%d", &n) != 1 || n < 0) return 1;

    for (int i = 0; i < n; i++) {
        int v; scanf("%d", &v);
        inserir_fim(&lista, v);
    }

    printf("Lista: ");
    imprimir(lista);

    printf("Inversa: ");
    imprimir_inversa(lista);
    printf("NULL\n");

    liberar(&lista);
    return 0;
}
