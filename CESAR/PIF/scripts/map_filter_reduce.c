#include <stdio.h>

int square(int x) { 
    return x * x; 
}

int is_odd(int x) { 
    return x % 2 != 0; 
}

int add(int acc, int x) { 
    return acc + x; 
}

int map_int(const int* in, int* out, int n, int (*f)(int)) {
    for (int i = 0; i < n; i++) out[i] = f(in[i]);
    return n;
}

int filter_int(const int* in, int* out, int n, int (*pred)(int)) {
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (pred(in[i])) {
            out[k++] = in[i];
        }
    }
    return k;
}

int reduce_int(const int* in, int n, int init, int (*op)(int, int)) {
    int acc = init;
    for (int i = 0; i < n; i++) acc = op(acc, in[i]);
    return acc;
}

int main() {
    int input[] = {1, 2, 3, 4, 5, 6, -7};
    
    int n = sizeof(input) / sizeof(input[0]);
    
    int out_map[n];
    int out_filter[n];
    
    // Inicializando arrays com zeros
    for(int i = 0; i < n; i++) {
        out_map[i] = 0;
        out_filter[i] = 0;
    }
    
    // Map: elevar ao quadrado
    map_int(input, out_map, n, square);
    printf("Map (x²): ");
    for (int i = 0; i < n; i++) printf("%d ", out_map[i]);
    printf("\n");
    
    // Filter: números ímpares
    int filter_count = filter_int(input, out_filter, n, is_odd);
    printf("Filter (ímpares): ");
    for (int i = 0; i < filter_count; i++) printf("%d ", out_filter[i]);
    printf("(count: %d)\n", filter_count);
    
    // Reduce: somar todos
    int sum = reduce_int(input, n, 0, add);
    printf("Reduce (soma): %d\n", sum);
    
    return 0;
}

/*
 Para executar via terminal:
    1. Salve o código em um arquivo chamado map_filter_reduce.c
    2. Compile o código usando o comando:
       gcc map_filter_reduce.c -o map_filter_reduce
    3. Execute o programa usando o comando:
         ./map_filter_reduce
    Você verá a saída:
    Map (x²): 1 4 9 16 25 36 49 
    Filter (ímpares): 1 3 5 -7 (count: 4)
    Reduce (soma): 14
*/
