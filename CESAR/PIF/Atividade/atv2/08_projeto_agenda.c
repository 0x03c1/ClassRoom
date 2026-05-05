/* =====================================================================
 * ATIVIDADE 8 - Projeto Integrador: Agenda de Contatos
 * ---------------------------------------------------------------------
 * Objetivo: Juntar TUDO do Módulo 8: ponteiros, alocação dinâmica
 *           e arquivos binários em um sistema funcional.
 *
 * Tempo estimado: 60 minutos (pode ser entrega para casa)
 * Compilar: gcc 08_projeto_agenda.c -o 08_projeto_agenda -Wall
 *
 * Cenário: Agenda de contatos com persistência em arquivo binário,
 *          carregada para um vetor dinâmico em memória ao iniciar.
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "agenda.dat"
#define MAX_NOME 50
#define MAX_TEL 20
#define CAPACIDADE_INICIAL 4

typedef struct {
    char nome[MAX_NOME];
    char telefone[MAX_TEL];
    int idade;
} Contato;

/* TODO 1: Implemente carregar_agenda() que:
 *         (a) abre ARQUIVO em "rb"
 *         (b) se não existir, retorna NULL e *qtd = 0 (agenda nova)
 *         (c) descobre quantos contatos há (use fseek/ftell)
 *         (d) aloca um vetor dinâmico desse tamanho
 *         (e) usa fread para carregar TODOS de uma vez
 *         (f) escreve a quantidade em *qtd
 *         (g) retorna o ponteiro
 *
 *         Assinatura: Contato* carregar_agenda(int *qtd, int *capacidade)
 *
 *         Detalhe: capacidade deve ser igual a qtd ao carregar
 *                  (você vai expandir conforme adicionar).
 */


/* TODO 2: Implemente salvar_agenda() que:
 *         (a) abre ARQUIVO em "wb" (sobrescreve)
 *         (b) usa fwrite para gravar todos os contatos de uma vez:
 *             fwrite(contatos, sizeof(Contato), qtd, fp);
 *         (c) fecha o arquivo
 *
 *         Assinatura: void salvar_agenda(Contato *contatos, int qtd)
 */


/* TODO 3: Implemente adicionar_contato() que:
 *         (a) verifica se qtd == capacidade
 *         (b) se sim, dobra a capacidade com realloc (use temp!)
 *         (c) pede dados do contato
 *         (d) coloca na posição qtd e incrementa
 *
 *         Assinatura: Contato* adicionar_contato(Contato *contatos,
 *                                                 int *qtd,
 *                                                 int *capacidade)
 *
 *         Por que retornar Contato*? Porque realloc pode mudar o
 *         endereço — o main precisa atualizar seu ponteiro.
 */


/* TODO 4: Implemente listar_contatos() que percorre o vetor e imprime.
 *
 *         Assinatura: void listar_contatos(Contato *contatos, int qtd)
 */


/* TODO 5: Implemente buscar_contato() que recebe um nome e:
 *         (a) procura no vetor (use strcmp)
 *         (b) imprime os dados ou "não encontrado"
 *
 *         Assinatura: void buscar_contato(Contato *contatos, int qtd)
 */


/* TODO 6 (DESAFIO): Implemente remover_contato() que:
 *         (a) recebe um nome
 *         (b) procura
 *         (c) se achou, "desloca" todos os subsequentes uma posição pra trás
 *             (loop ou memmove)
 *         (d) decrementa qtd
 *
 *         Assinatura: void remover_contato(Contato *contatos,
 *                                           int *qtd)
 */


int main(void) {
    int qtd = 0, capacidade = CAPACIDADE_INICIAL;
    Contato *contatos;
    int opcao;

    /* TODO 7: Carregue a agenda do arquivo. Se vier NULL (arquivo
     *         não existia), aloque um vetor inicial com CAPACIDADE_INICIAL.
     */


    do {
        printf("\n=== AGENDA (%d contatos) ===\n", qtd);
        printf("1. Adicionar contato\n");
        printf("2. Listar contatos\n");
        printf("3. Buscar contato\n");
        printf("4. Remover contato\n");
        printf("5. Salvar e sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        /* TODO 8: switch para chamar as funções */


    } while (opcao != 5);

    /* TODO 9: Salve a agenda em arquivo antes de sair. */


    /* TODO 10: LIBERE a memória do vetor.
     *          (Não esqueça! Sem isso, memory leak no fim do programa.)
     */


    printf("Agenda salva. Ate logo!\n");
    return 0;
}
