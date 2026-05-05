/* =====================================================================
 * ATIVIDADE 6 - Arquivos Texto: Escrever e Ler
 * ---------------------------------------------------------------------
 * Objetivo: Aprender o ciclo fopen -> usar -> fclose para arquivos
 *           texto. Praticar fprintf, fscanf, fgets.
 *
 * Tempo estimado: 30 minutos
 * Compilar: gcc 06_arquivos_texto.c -o 06_arquivos_texto -Wall
 *
 * Cenário: Cadastro simples de notas de alunos em arquivo texto.
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define ARQUIVO "notas.txt"

/* TODO 1: Implemente cadastrar_alunos() que:
 *         (a) abre ARQUIVO em modo "w" (escrita - sobrescreve)
 *         (b) verifica se fopen retornou NULL (erro)
 *         (c) pergunta quantos alunos serão cadastrados
 *         (d) para cada aluno, lê NOME e NOTA e grava no arquivo
 *             usando fprintf no formato: "nome nota\n"
 *         (e) fecha o arquivo com fclose
 *
 *         Assinatura: void cadastrar_alunos(void)
 *
 *         Dica: para ler nome com espaços, use fgets do stdin.
 *         Para simplicidade, peça nomes de uma palavra só e use scanf.
 */


/* TODO 2: Implemente listar_alunos() que:
 *         (a) abre ARQUIVO em modo "r" (leitura)
 *         (b) verifica se fopen retornou NULL (arquivo pode não existir)
 *         (c) lê linha a linha com fscanf até o fim (EOF)
 *         (d) imprime cada aluno na tela formatado
 *         (e) fecha o arquivo
 *
 *         Assinatura: void listar_alunos(void)
 *
 *         Padrão para ler até o fim:
 *         while (fscanf(fp, "%s %f", nome, &nota) == 2) { ... }
 *
 *         O fscanf retorna o número de campos lidos com sucesso.
 *         Quando der EOF, retorna EOF (-1) e o while termina.
 */


/* TODO 3: Implemente calcular_media_turma() que:
 *         (a) abre ARQUIVO em modo "r"
 *         (b) lê todas as notas, soma e conta
 *         (c) imprime a média e quantos alunos foram processados
 *         (d) fecha o arquivo
 *
 *         Assinatura: void calcular_media_turma(void)
 *
 *         Cuidado: se o arquivo não tiver alunos, evite divisão por zero.
 */


int main(void) {
    int opcao;

    do {
        printf("\n=== MENU ===\n");
        printf("1. Cadastrar alunos\n");
        printf("2. Listar alunos\n");
        printf("3. Calcular media da turma\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        /* TODO 4: Use switch para chamar a função correspondente.
         *         Para opções inválidas, imprima mensagem.
         */


    } while (opcao != 0);

    return 0;
}
