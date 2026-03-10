#include <stdio.h>
#include "matematica.h"  /* Aspas = arquivo local; <> = sistema */

int main(void) {
    printf("5! = %d\n", fatorial(5));
    printf("2^10 = %.0f\n", potencia(2, 10));
    printf("17 é primo? %s\n", eh_primo(17) ? "Sim" : "Não");
    return 0;
}

/* Compilação:
 *   gcc -Wall -c matematica.c     -> gera matematica.o
 *   gcc -Wall -c main.c           -> gera main.o
 *   gcc -o programa main.o matematica.o
 *
 * Ou tudo de uma vez:
 *   gcc -Wall -o programa main.c matematica.c
 */
