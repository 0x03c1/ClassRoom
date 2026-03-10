#include <stdio.h>
#include <string.h>

/* Definição de um tipo estruturado */
typedef struct {
    char  nome[50];
    int   idade;
    float nota;
} Aluno;

/* Enum: conjunto nomeado de constantes */
typedef enum {
    APROVADO,
    RECUPERACAO,
    REPROVADO
} Situacao;

Situacao avaliar(float nota) {
    if (nota >= 7.0f) return APROVADO;
    if (nota >= 5.0f) return RECUPERACAO;
    return REPROVADO;
}

const char* situacao_str(Situacao s) {
    switch (s) {
        case APROVADO:    return "Aprovado";
        case RECUPERACAO: return "Recuperação";
        case REPROVADO:   return "Reprovado";
        default:          return "Desconhecido";
    }
}

int main(void) {
    Aluno turma[3] = {
        {"Ana",    21, 8.5f},
        {"Bruno",  19, 4.2f},
        {"Carlos", 20, 6.0f}
    };

    printf("%-10s %5s %5s  %-12s\n", "Nome", "Idade", "Nota", "Situação");
    printf("----------------------------------------\n");

    for (int i = 0; i < 3; i++) {
        Situacao sit = avaliar(turma[i].nota);
        printf("%-10s %5d %5.1f  %-12s\n",
               turma[i].nome, turma[i].idade,
               turma[i].nota, situacao_str(sit));
    }

    return 0;
}
