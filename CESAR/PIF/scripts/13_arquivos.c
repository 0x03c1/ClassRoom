#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    float nota;
} Aluno;

int main(void) {
    /* ── Escrita ── */
    FILE *arq = fopen("alunos.txt", "w");  /* "w" = write (cria/sobrescreve) */
    if (arq == NULL) {
        perror("Erro ao abrir arquivo");   /* perror mostra mensagem do sistema */
        return 1;
    }

    Aluno turma[] = {{"Ana", 8.5f}, {"Bruno", 7.0f}, {"Clara", 9.2f}};
    int n = sizeof(turma) / sizeof(turma[0]);

    for (int i = 0; i < n; i++) {
        fprintf(arq, "%s %.1f\n", turma[i].nome, turma[i].nota);
    }
    fclose(arq);  /* SEMPRE feche! Dados podem não ser gravados sem isso */
    printf("Dados salvos com sucesso.\n");

    /* ── Leitura ── */
    arq = fopen("alunos.txt", "r");  /* "r" = read */
    if (arq == NULL) {
        perror("Erro ao ler arquivo");
        return 1;
    }

    char nome[50];
    float nota;
    printf("\nDados lidos:\n");
    while (fscanf(arq, "%49s %f", nome, &nota) == 2) {
        printf("  %-10s -> %.1f\n", nome, nota);
    }
    fclose(arq);

    return 0;
}
