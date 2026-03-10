#include "matematica.h"

int fatorial(int n) {
    if (n <= 1) return 1;
    return n * fatorial(n - 1);
}

double potencia(double base, int exp) {
    double resultado = 1.0;
    for (int i = 0; i < exp; i++)
        resultado *= base;
    return resultado;
}

int eh_primo(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}
