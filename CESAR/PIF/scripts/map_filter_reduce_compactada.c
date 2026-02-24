#include <stdio.h>

int positivo(int x) { return x > 0; }
int quadrado(int x) { return x * x; }
int soma(int a, int b) { return a + b; }
int is_odd(int x) { return x % 2 != 0; } // Mantido para o seu teste de ímpares

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

// Abordagem Imperativa
int soma_quadrados_positivos_imp(const int* v, int n) {
    int acc = 0;
    for (int i = 0; i < n; i++) {
        if (positivo(v[i])) { 
            acc = soma(acc, quadrado(v[i])); 
        }
    }
    return acc;
}

// Abordagem Funcional (Aproveitando seu map, filter e reduce)
int soma_quadrados_positivos_fun(const int* v, int n) {
    int buf_filter[n];
    int buf_map[n];
    
    int count = filter_int(v, buf_filter, n, positivo);
    map_int(buf_filter, buf_map, count, quadrado);
    return reduce_int(buf_map, count, 0, soma);
}

int main() {
    // Coloquei números negativos para testar o filtro de positivos
    int input[] = {-2, -1, 1, 2, 3};
    int n = 5;
    
    int out_map[5] = {0};
    int out_filter[5] = {0};
    
    printf("--- TESTES ISOLADOS ---\n");

    // Map: elevar ao quadrado usando a nova função 'quadrado'
    map_int(input, out_map, n, quadrado);
    printf("Map (x²): ");
    for(int i = 0; i < n; i++) printf("%d ", out_map[i]);
    printf("\n");
    
    // Filter: números ímpares
    int filter_count = filter_int(input, out_filter, n, is_odd);
    printf("Filter (ímpares): ");
    for (int i = 0; i < filter_count; i++) printf("%d ", out_filter[i]);
    printf("(count: %d)\n", filter_count);
    
    // Reduce: somar todos usando a nova função 'soma'
    int total = reduce_int(input, n, 0, soma);
    printf("Reduce (soma total): %d\n", total);
    
    printf("\n--- COMPOSICAO: SOMA DOS QUADRADOS DOS POSITIVOS ---\n");
    
    // Testando as funções que você pediu
    int res_imp = soma_quadrados_positivos_imp(input, n);
    int res_fun = soma_quadrados_positivos_fun(input, n);
    
    // Positivos: 1, 2, 3 -> Quadrados: 1, 4, 9 -> Soma: 14
    printf("Resultado Imperativo: %d\n", res_imp);
    printf("Resultado Funcional: %d\n", res_fun);

    return 0;
}

/*
    Para compilar e rodar:
        1. Salve o código em um arquivo chamado map_filter_reduce_compactada.c
        2. Compile usando:
        gcc map_filter_reduce_compactada.c -o map_filter_reduce_compactada
        3. Execute usando:
        ./map_filter_reduce_compactada
        Você verá a saída com os testes isolados e a composição final.
*/
