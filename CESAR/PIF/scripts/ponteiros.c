#include <stdio.h>

int aplica(int x, int (*f)(int)) {
    return f(x);
}

int dobro(int x) { return 2 * x; }

int main() {
    int r = aplica(10, dobro);
    printf("Resultado: %d\n", r);
    return 0;
}
/*
 Para executar via terminal:
    1. Salve o código em um arquivo chamado ponteiros.c
    2. Compile o código usando o comando:
       gcc ponteiros.c -o ponteiros
    3. Execute o programa usando o comando:
         ./ponteiros
    Você verá a saída:
    Resultado: 20
*/
