#include <stdio.h>

int main(void) {
    int senha, tentativas = 0;
    const int SENHA_CORRETA = 1234;

    /* do-while: garante ao menos 1 execução */
    do {
        printf("Digite a senha: ");
        scanf("%d", &senha);
        tentativas++;

        if (senha != SENHA_CORRETA) {
            printf("Senha incorreta. Tente novamente.\n");
        }
    } while (senha != SENHA_CORRETA && tentativas < 3);

    if (senha == SENHA_CORRETA) {
        printf("Acesso liberado após %d tentativa(s).\n", tentativas);
    } else {
        printf("Bloqueado após %d tentativas.\n", tentativas);
    }

    return 0;
}
