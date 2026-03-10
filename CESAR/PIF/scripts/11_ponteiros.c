#include <stdio.h>

int main(void) {
    int valor = 42;
    int *ptr = &valor;   /* ptr armazena o endereço de valor */

    printf("valor       = %d\n", valor);         /* 42 */
    printf("&valor      = %p\n", (void*)&valor);  /* endereço (ex: 0x7ffd...) */
    printf("ptr         = %p\n", (void*)ptr);     /* mesmo endereço */
    printf("*ptr        = %d\n", *ptr);           /* 42 (desreferenciação) */

    *ptr = 100;  /* Modifica valor ATRAVÉS do ponteiro */
    printf("valor agora = %d\n", valor);  /* 100 */

    /* Ponteiro e arrays: relação íntima */
    int nums[] = {10, 20, 30, 40, 50};
    int *p = nums;  /* Array decai para ponteiro automaticamente */

    for (int i = 0; i < 5; i++) {
        printf("*(p+%d) = %d  |  p[%d] = %d\n", i, *(p+i), i, p[i]);
        /* Aritmética de ponteiros: p+i avança i * sizeof(int) bytes */
    }

    return 0;
}
