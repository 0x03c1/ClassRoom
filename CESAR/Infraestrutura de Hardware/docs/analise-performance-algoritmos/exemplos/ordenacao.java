/*
 * Exemplo de algoritmo para o comparador_stacks.py
 * Mesma operação que ordenacao.py e ordenacao.c, mas em Java.
 *
 * Usa pivô por mediana-de-três para reduzir o pior caso do quicksort
 * em entradas já ordenadas (mantém comparabilidade entre as 3 linguagens).
 *
 * Compilar:  javac Ordenacao.java
 * Uso:       java Ordenacao <tamanho>
 */

import java.util.Random;

public class Ordenacao {

    static void quicksort(int[] arr, int lo, int hi) {
        if (lo < hi) {
            // Pivô = mediana-de-três (lo, mid, hi). Move a mediana para arr[hi].
            int mid = (lo + hi) >>> 1;
            if (arr[lo] > arr[mid]) { int t = arr[lo]; arr[lo] = arr[mid]; arr[mid] = t; }
            if (arr[lo] > arr[hi])  { int t = arr[lo]; arr[lo] = arr[hi];  arr[hi]  = t; }
            if (arr[mid] > arr[hi]) { int t = arr[mid]; arr[mid] = arr[hi]; arr[hi] = t; }
            int tmp = arr[mid]; arr[mid] = arr[hi]; arr[hi] = tmp;

            int pivot = arr[hi];
            int i = lo - 1;
            for (int j = lo; j < hi; j++) {
                if (arr[j] <= pivot) {
                    i++;
                    int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
                }
            }
            int t = arr[i + 1]; arr[i + 1] = arr[hi]; arr[hi] = t;
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
