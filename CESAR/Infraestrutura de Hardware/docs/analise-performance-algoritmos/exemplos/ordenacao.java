/*
 * Exemplo de algoritmo para o comparador_stacks.py
 * Mesma operação que ordenacao.py e ordenacao.c, mas em Java.
 *
 * Compilar:  javac Ordenacao.java
 * Uso:       java Ordenacao <tamanho>
 */

import java.util.Random;

public class Ordenacao {
    static void quicksort(int[] arr, int lo, int hi) {
        if (lo < hi) {
            int pivot = arr[hi];
            int i = lo - 1;
            for (int j = lo; j < hi; j++) {
                if (arr[j] <= pivot) {
                    i++;
                    int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
                }
            }
            int tmp = arr[i + 1]; arr[i + 1] = arr[hi]; arr[hi] = tmp;
            int p = i + 1;
            quicksort(arr, lo, p - 1);
            quicksort(arr, p + 1, hi);
        }
    }

    public static void main(String[] args) {
        if (args.length < 1) {
            System.err.println("Uso: java Ordenacao <tamanho>");
            System.exit(1);
        }
        int n = Integer.parseInt(args[0]);
        Random rng = new Random(42);
        int[] arr = new int[n];
        for (int i = 0; i < n; i++) arr[i] = rng.nextInt(1_000_001);

        quicksort(arr, 0, n - 1);

        System.out.println(arr[0] + " ... " + arr[n - 1]);
    }
}
