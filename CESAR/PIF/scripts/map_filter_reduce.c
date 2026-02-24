#include <stdio.h>

#define TAMANHO 7

int quadrado(int x) { 
    return x * x; 
}

int eh_impar(int x) { 
    return x % 2 != 0; 
}

// MAP: Aplica uma transformação (elevar ao quadrado) em todos os elementos
void map_quadrado(int entrada[], int saida[], int n) {
    for (int i = 0; i < n; i++) {
        saida[i] = quadrado(entrada[i]);
    }
}

// FILTER: Copia para a saída apenas os elementos que passam no teste (ser ímpar)
int filter_impar(int entrada[], int saida[], int n) {
    int contagem = 0; // Controla em qual posição da saída vamos gravar
    
    for (int i = 0; i < n; i++) {
        if (eh_impar(entrada[i])) {
            saida[contagem] = entrada[i];
            contagem++;
        }
    }
    return contagem; // Retorna quantos itens passaram no filtro
}

// REDUCE: Acumula todos os valores em um único resultado (somar)
int reduce_soma(int entrada[], int n) {
    int soma = 0; // Nosso acumulador inicial
    
    for (int i = 0; i < n; i++) {
        soma = soma + entrada[i];
    }
    return soma;
}

int main() {
    int input[TAMANHO] = {1, 2, 3, 4, 5, 6, -7};
    
    // Arrays para guardar os resultados
    int out_map[TAMANHO];
    int out_filter[TAMANHO];
    
    // --- Testando o MAP ---
    map_quadrado(input, out_map, TAMANHO);
    
    printf("Map (x ao quadrado): ");
    for (int i = 0; i < TAMANHO; i++) {
        printf("%d ", out_map[i]);
    }
    printf("\n");
    
    
    // --- Testando o FILTER ---
    int qtd_filtrados = filter_impar(input, out_filter, TAMANHO);
    
    printf("Filter (apenas impares): ");
    // Note que iteramos apenas até 'qtd_filtrados', não até TAMANHO
    for (int i = 0; i < qtd_filtrados; i++) {
        printf("%d ", out_filter[i]);
    }
    printf("\n");
    
    
    // --- Testando o REDUCE ---
    int resultado_soma = reduce_soma(input, TAMANHO);
    
    printf("Reduce (soma de todos): %d\n", resultado_soma);
    
    return 0;
}
