#include <stdio.h>

long fatorial(int n)
{
    long resultado = 1;
    for (int i = 2; i <= n; i++){
        resultado = resultado * i;
    }
    return resultado;
}

int main()
{
    int n;

    printf("========================================\n");
    printf("  Cálculo do Fatorial de N\n");
    printf("========================================\n");

    printf("Digite um número: ");
    scanf("%d", &n);

    if (n < 0)
    {
        printf("Erro: fatorial não é definido para negativos.\n");
    }
    else
    {
        printf("Fatorial de %d = %ld\n", n, fatorial(n));
    }

    return 0;
}
