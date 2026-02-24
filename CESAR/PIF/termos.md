# Termos de Programação Imperativa e Funcional em C

## Paradigmas de Programação

- **Imperative Programming** (Programação Imperativa): Paradigma de programação que enfatiza como resolver um problema, através de sequência explícita de comandos que alteram o estado do programa.
  
  Exemplo: Usar loops e condições para iterar sobre dados e modificar variáveis.
  ```c
  int sum = 0;
  for (int i = 0; i < 10; i++) {
      sum += i;  // Alterando o estado de sum
  }
  ```

- **Functional Programming** (Programação Funcional): Paradigma que trata a computação como a avaliação de funções matemáticas, evitando mudanças de estado e dados mutáveis.
  
  Exemplo: Usar funções puras que retornam valores sem alterar o estado externo.
  ```c
  int add(int a, int b) {
      return a + b;  // Sem efeitos colaterais
  }
  ```

- **Declarative Programming** (Programação Declarativa): Abordagem que especifica o que deve ser feito sem descrever explicitamente como fazê-lo.
  
  Exemplo: Descrever objetivos ao invés de passos específicos.

## Conceitos Fundamentais de Programação Imperativa

- **Variable** (Variável): Nomeação de um local de memória que armazena um valor que pode ser modificado durante a execução do programa.
  
  Exemplo:
  ```c
  int contador = 0;
  contador = 10;  // Valor alterado
  ```

- **State** (Estado): Conjunto de valores de todas as variáveis em um ponto específico da execução do programa.
  
  Exemplo: Em um programa de banco, o estado inclui o saldo da conta que muda com transações.

- **Side Effect** (Efeito Colateral): Qualquer mudança no estado do programa ou do mundo externo realizada por uma função.
  
  Exemplo:
  ```c
  void increment(int *x) {
      (*x)++;  // Modifica a variável externa
  }
  ```

- **Mutation** (Mutação): Alteração do valor de uma variável ou estrutura de dados após sua criação.
  
  Exemplo:
  ```c
  int arr[5] = {1, 2, 3, 4, 5};
  arr[0] = 10;  // Mutação do array
  ```

- **Loop** (Laço): Estrutura de controle que repete um bloco de código enquanto uma condição for verdadeira.
  
  Exemplo:
  ```c
  for (int i = 0; i < 5; i++) {
      printf("%d\n", i);
  }
  ```

- **Conditional Statement** (Comando Condicional): Estrutura que executa diferentes blocos de código baseado em condições.
  
  Exemplo:
  ```c
  if (age >= 18) {
      printf("Maior de idade\n");
  } else {
      printf("Menor de idade\n");
  }
  ```

- **Sequence** (Sequência): Ordem em que as instruções são executadas no programa.
  
  Exemplo: Primeira declaração de variável, depois atribuição, depois impressão.

## Conceitos Fundamentais de Programação Funcional

- **Pure Function** (Função Pura): Função cuja saída depende apenas de suas entradas e não realiza efeitos colaterais.
  
  Exemplo:
  ```c
  int multiply(int a, int b) {
      return a * b;  // Saída determinística, sem efeitos colaterais
  }
  ```

- **Immutability** (Imutabilidade): Propriedade de dados que não podem ser alterados após sua criação.
  
  Exemplo: Usar const para garantir que valores não sejam modificados.
  ```c
  const int MAX = 100;  // Não pode ser alterado
  ```

- **First-Class Function** (Função de Primeira Classe): Função que pode ser atribuída a variáveis, passada como argumento ou retornada por outras funções.
  
  Exemplo:
  ```c
  int (*func_ptr)(int, int) = &add;  // Ponteiro para função
  ```

- **Higher-Order Function** (Função de Ordem Superior): Função que recebe outra função como argumento ou retorna uma função.
  
  Exemplo:
  ```c
  void apply_twice(int (*func)(int), int value) {
      func(func(value));
  }
  ```

- **Map** (Mapeamento): Aplicar uma função a cada elemento de uma coleção, produzindo uma nova coleção.
  
  Exemplo:
  ```c
  void map(int arr[], int size, int (*func)(int)) {
      for (int i = 0; i < size; i++) {
          arr[i] = func(arr[i]);
      }
  }
  ```

- **Filter** (Filtragem): Selecionar elementos de uma coleção que satisfazem uma condição.
  
  Exemplo:
  ```c
  int filtered[100];
  int count = 0;
  for (int i = 0; i < size; i++) {
      if (is_even(arr[i])) {
          filtered[count++] = arr[i];
      }
  }
  ```

- **Reduce** (Redução): Combinar todos os elementos de uma coleção em um único valor.
  
  Exemplo:
  ```c
  int sum = 0;
  for (int i = 0; i < size; i++) {
      sum = add(sum, arr[i]);  // Reduzindo array a um valor
  }
  ```

- **Recursion** (Recursão): Técnica onde uma função chama a si mesma para resolver um problema.
  
  Exemplo:
  ```c
  int factorial(int n) {
      if (n <= 1) return 1;
      return n * factorial(n - 1);
  }
  ```

- **Tail Recursion** (Recursão de Cauda): Forma de recursão onde a chamada recursiva é a última operação da função.
  
  Exemplo:
  ```c
  int tail_factorial(int n, int acc) {
      if (n <= 1) return acc;
      return tail_factorial(n - 1, n * acc);
  }
  ```

- **Lambda Function** (Função Anônima): Função sem nome, tipicamente usada para operações simples e imediatas.
  
  Exemplo (em C moderno com macros):
  ```c
  #define LAMBDA(x) (x * x)
  int result = LAMBDA(5);  // Simples operação anônima
  ```

## Estruturas de Dados Funcionais

- **List** (Lista): Sequência de elementos onde operações funcionais podem ser aplicadas.
  
  Exemplo: Array em C que pode ser processado com map, filter, reduce.

- **Linked List** (Lista Encadeada): Estrutura de dados onde cada nó contém um valor e referência para o próximo.
  
  Exemplo:
  ```c
  struct Node {
      int data;
      struct Node *next;
  };
  ```

- **Tuple** (Tupla): Coleção imutável de elementos de tipos heterogêneos.
  
  Exemplo:
  ```c
  struct Pair {
      int first;
      char second;
  };
  ```

- **Immutable Data Structure** (Estrutura de Dados Imutável): Estrutura que não muda após criação, permitindo operações que retornam novas instâncias.
  
  Exemplo: Usar const em estruturas para garantir imutabilidade.

## Técnicas e Padrões

- **Closure** (Encapsulamento): Função que tem acesso às variáveis do escopo onde foi definida.
  
  Exemplo:
  ```c
  int *create_multiplier(int factor) {
      int *ptr = malloc(sizeof(int));
      *ptr = factor;  // Captura o contexto
      return ptr;
  }
  ```

- **Callback** (Chamada de Retorno): Função passada como argumento para ser executada posteriormente.
  
  Exemplo:
  ```c
  void process_array(int arr[], int size, void (*callback)(int)) {
      for (int i = 0; i < size; i++) {
          callback(arr[i]);
      }
  }
  ```

- **Composition** (Composição de Funções): Combinar funções simples para criar funções mais complexas.
  
  Exemplo:
  ```c
  int composed(int x) {
      return add_one(multiply_by_two(x));
  }
  ```

- **Memoization** (Memorização): Técnica de otimização que armazena resultados de funções para evitar recálculos.
  
  Exemplo:
  ```c
  int memo[100] = {0};
  int fib(int n) {
      if (memo[n] != 0) return memo[n];
      if (n <= 1) return n;
      memo[n] = fib(n-1) + fib(n-2);
      return memo[n];
  }
  ```

- **Lazy Evaluation** (Avaliação Preguiçosa): Atrasar o cálculo de um valor até que seja necessário.
  
  Exemplo: Usar funções que computam valores sob demanda.

## Conceitos Avançados

- **Type System** (Sistema de Tipos): Conjunto de regras que definem como tipos de dados são usados e combinados.
  
  Exemplo: C é fortemente tipado, exigindo conversões explícitas entre tipos.

- **Polymorphism** (Polimorfismo): Capacidade de usar uma interface única para diferentes tipos de dados.
  
  Exemplo:
  ```c
  void *generic_swap(void *a, void *b, size_t size) {
      // Troca genérica para qualquer tipo
  }
  ```

- **Pattern Matching** (Correspondência de Padrões): Técnica de decompor estruturas de dados e extrair valores.
  
  Exemplo: Verificar estruturas de dados e extrair campos específicos.

- **Lazy List** (Lista Preguiçosa): Lista que gera elementos sob demanda, não armazenando todos em memória.
  
  Exemplo: Implementar geradores que produzem sequências infinitas.

- **Currying** (Aplicação Parcial): Converter uma função que recebe múltiplos argumentos em uma série de funções que recebem um argumento.
  
  Exemplo:
  ```c
  int (*add_with_5)(int) = create_adder(5);
  int result = add_with_5(10);  // Retorna 15
  ```

- **Partial Application** (Aplicação Parcial): Fixar alguns argumentos de uma função para criar uma nova função.
  
  Exemplo:
  ```c
  typedef int (*BinaryOp)(int, int);
  int multiply_by_3(int x) {
      return x * 3;  // Aplicação parcial de multiply
  }
  ```

## Comparação entre Paradigmas

- **Imperative vs Functional**: Imperativo foca no como (sequência de passos), Funcional foca no quê (transformações de dados).
  
  Exemplo imperativo:
  ```c
  int sum = 0;
  for (int i = 0; i < n; i++) sum += arr[i];
  ```
  
  Exemplo funcional (usando reduce):
  ```c
  int sum = reduce(arr, n, 0, add);
  ```

- **Stateful vs Stateless**: Imperativo mantém estado mutável, Funcional mantém estado imutável.

- **Procedural vs Declarative**: Procedural descreve passos, Declarative descreve resultado desejado.
