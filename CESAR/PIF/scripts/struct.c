#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[100];
    int matricula;
    float notas[3];
    float media;
    char situacao[15];
} Aluno;

void calcularMedia(Aluno *a) {
    a->media = (a->notas[0] + a->notas[1] + a->notas[2]) / 3.0;
    if (a->media >= 7.0)
        strcpy(a->situacao, "Aprovado");
    else if (a->media >= 5.0)
        strcpy(a->situacao, "Recuperacao");
    else
        strcpy(a->situacao, "Reprovado");
}

int main() {
    Aluno aluno;
    
    strcpy(aluno.nome, "João Silva");
    aluno.matricula = 12345;
    aluno.notas[0] = 8.5;
    aluno.notas[1] = 7.0;
    aluno.notas[2] = 9.0;
    
    calcularMedia(&aluno);
    
    printf("Aluno: %s\n", aluno.nome);
    printf("Matrícula: %d\n", aluno.matricula);
    printf("Média: %.2f\n", aluno.media);
    printf("Situação: %s\n", aluno.situacao);
    
    return 0;
}
