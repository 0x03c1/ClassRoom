#include <stdio.h>

int soma_ate_n(int n) {
    int soma = 0;               // estado inicial
    for (int i = 1; i <= n; i++) {
        soma = soma + i;        // mutação de estado
    }
    return soma;
}

int main() {
    int n = 5;
    int resultado = soma_ate_n(n);
    printf("A soma dos números de 1 a %d é: %d\n", n, resultado);
    return 0;
}

/*
 Para executar via terminal:
    1. Salve o código em um arquivo chamado soma.c
    2. Compile o código usando o comando:
       gcc soma.c -o soma
    3. Execute o programa usando o comando:
         ./soma
    Você verá a saída:
    A soma dos números de 1 a 5 é: X
*/
