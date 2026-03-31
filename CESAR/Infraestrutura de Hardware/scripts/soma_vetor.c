#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define N 1000000000L  // 1 bilhão

int main(void) {
    struct timespec t_start, t_end;

    clock_gettime(CLOCK_MONOTONIC, &t_start);

    /*
     * volatile impede o compilador de "trapacear":
     *   - Sem volatile + O3, o GCC substitui o loop inteiro
     *     pela fórmula N*(N+1)/2 (loop elimination)
     *   - Com volatile, o compilador é FORÇADO a executar
     *     cada iteração, simulando trabalho real
     *
     * EXERCÍCIO PARA OS ALUNOS:
     *   Remova o "volatile" e compile com -O3.
     *   O que acontece com o tempo? Por quê?
     */
    volatile int64_t soma = 0;

    for (int64_t i = 1; i <= N; i++) {
        soma += i;
    }

    // Marca fim
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    // Calcula tempo decorrido em milissegundos
    double elapsed_ms = (t_end.tv_sec - t_start.tv_sec) * 1000.0
                      + (t_end.tv_nsec - t_start.tv_nsec) / 1e6;

    printf("Soma = %ld\n", (long)soma);
    printf("Tempo: %.2f ms\n", elapsed_ms);
    printf("N = %ld iterações\n", N);

    return 0;
}
