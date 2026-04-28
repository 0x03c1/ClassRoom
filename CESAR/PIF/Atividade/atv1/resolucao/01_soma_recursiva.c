#include <stdio.h>

void soma_ate(int n, long *acc) {
  if (n == 0) {
    return;
  }
  long x = *acc + n;
  *acc = x;
  soma_ate(n - 1, acc);
}

int main(void) {
  int n;
  long resultado = 0;

  printf("Digite N: ");
  if (scanf("%d", &n) != 1 || n < 0) {
    printf("Entrada invalida.\n");
    return 1;
  }

  soma_ate(n, &resultado);
  printf("Soma de 1 a %d = %ld\n", n, resultado);
  return 0;
}
