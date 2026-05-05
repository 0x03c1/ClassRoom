/* =====================================================================
 * ATIVIDADE 7 - Arquivos Binários: Persistindo Structs
 * ---------------------------------------------------------------------
 * Objetivo: Gravar e ler structs em arquivo BINÁRIO usando
 *           fwrite e fread. Comparar com arquivo texto.
 *
 * Tempo estimado: 35 minutos
 * Compilar: gcc 07_arquivos_binarios.c -o 07_arquivos_binarios -Wall
 *
 * Cenário: Cadastro de produtos de uma loja.
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "produtos.dat"

typedef struct {
    int codigo;
    char nome[40];
    float preco;
    int estoque;
} Produto;

/* TODO 1: Implemente adicionar_produto() que:
 *         (a) abre ARQUIVO em modo "ab" (append binário)
 *             - "a" = adiciona no final, não sobrescreve!
 *             - "b" = binário
 *         (b) verifica se fopen falhou
 *         (c) pede ao usuário os dados de UM produto
 *         (d) usa fwrite para gravar a struct INTEIRA de uma vez:
 *             fwrite(&p, sizeof(Produto), 1, fp);
 *         (e) fecha o arquivo
 *
 *         Assinatura: void adicionar_produto(void)
 *
 *         Dica para ler nome com espaços:
 *         scanf(" %39[^\n]", p.nome);
 *         (lê até newline, máx 39 chars, com espaço inicial pra ignorar
 *          o '\n' deixado pelo scanf anterior)
 */


/* TODO 2: Implemente listar_produtos() que:
 *         (a) abre ARQUIVO em modo "rb"
 *         (b) se NULL, avisa que não há produtos cadastrados
 *         (c) lê produto por produto com fread em um loop:
 *             while (fread(&p, sizeof(Produto), 1, fp) == 1) { ... }
 *         (d) imprime cada produto formatado
 *         (e) fecha o arquivo
 *
 *         Assinatura: void listar_produtos(void)
 */


/* TODO 3: Implemente buscar_produto() que:
 *         (a) recebe um código do usuário
 *         (b) percorre o arquivo até encontrar produto com aquele código
 *         (c) imprime os dados se encontrou, ou "não encontrado"
 *
 *         Assinatura: void buscar_produto(void)
 */


/* TODO 4: Implemente contar_produtos() que retorna quantos produtos
 *         há no arquivo SEM ler todos eles.
 *
 *         Assinatura: int contar_produtos(void)
 *
 *         Truque profissional:
 *         (a) abre em "rb"
 *         (b) fseek(fp, 0, SEEK_END);   // vai pro fim
 *         (c) long tamanho = ftell(fp); // pega posição (= tamanho)
 *         (d) fclose(fp);
 *         (e) return tamanho / sizeof(Produto);
 *
 *         Por que isso funciona? Porque cada produto ocupa EXATAMENTE
 *         sizeof(Produto) bytes em arquivo binário.
 */


int main(void) {
    int opcao;

    do {
        printf("\n=== LOJA ===\n");
        printf("1. Adicionar produto\n");
        printf("2. Listar produtos\n");
        printf("3. Buscar produto por codigo\n");
        printf("4. Total de produtos cadastrados\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        /* TODO 5: switch chamando as funções */


    } while (opcao != 0);

    return 0;
}
