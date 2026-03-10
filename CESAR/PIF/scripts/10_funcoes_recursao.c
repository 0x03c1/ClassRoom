#include <stdio.h>

/* Protótipos (declarações antecipadas) */
int  fatorial(int n);
void trocar(int *a, int *b);  /* Passagem por referência via ponteiro */
int  max(int a, int b);

int main(void) {
    printf("5! = %d\n", fatorial(5));  /* 120 */

    int x = 10, y = 20;
    printf("Antes: x=%d, y=%d\n", x, y);
    trocar(&x, &y);  /* Passa endereços */
    printf("Depois: x=%d, y=%d\n", x, y);

    printf("Maior: %d\n", max(42, 17));

    return 0;
}

/* Recursão: função que chama a si mesma */
int fatorial(int n) {
    if (n <= 1) return 1;        /* Caso base: ESSENCIAL! */
    return n * fatorial(n - 1);   /* Caso recursivo */
}

/* Troca usando ponteiros (única forma em C) */
void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int max(int a, int b) {
    return (a > b) ? a : b;  /* Operador ternário */
}
