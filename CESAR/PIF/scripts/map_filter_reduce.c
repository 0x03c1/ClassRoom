#include <stdio.h>

// 1. Em C, somos obrigados a dar nome e declarar as funções fora do main:
int square(int x) { return x * x; }
int is_odd(int x) { return x % 2 == 1; }
int add(int acc, int x) { return acc + x; }

int map_int(const int* in, int* out, int n, int (*f)(int)) {
    for (int i = 0; i < n; i++) out[i] = f(in[i]);
    return n;
}

int filter_int(const int* in, int* out, int n, int (*pred)(int)) {
    int k = 0;
    for (int i = 0; i < n; i++) if (pred(in[i])) out[k++] = in[i];
    return k;
}

int reduce_int(const int* in, int n, int init, int (*op)(int,int)) {
    int acc = init;
    for (int i = 0; i < n; i++) acc = op(acc, in[i]);
    return acc;
}

int main() {
    int input[] = {1, 2, 3};
    int out_map[3] = {0};
    int out_filter[3] = {0};
    
    // 2. Passamos apenas o nome das funções como argumento:
    map_int(input, out_map, 3, square);
    printf("Map (x²): %d, %d, %d\n", out_map[0], out_map[1], out_map[2]);
    
    int filter_count = filter_int(input, out_filter, 3, is_odd);
    printf("Filter (ímpares): ");
    for (int i = 0; i < filter_count; i++) printf("%d ", out_filter[i]);
    printf("(count: %d)\n", filter_count);
    
    int sum = reduce_int(input, 3, 0, add);
    printf("Reduce (soma): %d\n", sum);
    
    return 0;
}
