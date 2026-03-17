#include <stdio.h>

int eh_primo(int n) {
    if (n <= 1) {
        return 0;   // 0 e 1 não são primos
    }
    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) {
            return 0;   // encontrou divisor → não é primo
        }
    }
    return 1;   // nenhum divisor encontrado → é primo
}

int main() {
    int n;

    printf("========================================\n");
    printf("  Verificação de Número Primo\n");
    printf("========================================\n");

    printf("Digite um número: ");
    scanf("%d", &n);

    if (eh_primo(n)) {
        printf("%d é primo!\n", n);
    } else {
        printf("%d NÃO é primo.\n", n);
    }

    return 0;
}
