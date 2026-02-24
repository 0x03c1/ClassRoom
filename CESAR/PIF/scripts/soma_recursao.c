#include <stdio.h>

int soma_ate_n_rec(int n) {
    if (n <= 0) return 0;      // caso base
    return n + soma_ate_n_rec(n - 1);
}

int main() {
    int n = 5;
    int resultado = soma_ate_n_rec(n);
    printf("A soma dos números de 1 a %d é: %d\n", n, resultado);
    return 0;
}

/*
 Para executar via terminal:
    1. Salve o código em um arquivo chamado soma_recursao.c
    2. Compile o código usando o comando:
       gcc soma_recursao.c -o soma_recursao
    3. Execute o programa usando o comando:
         ./soma_recursao
    Você verá a saída:
    A soma dos números de 1 a 5 é: X
*/
