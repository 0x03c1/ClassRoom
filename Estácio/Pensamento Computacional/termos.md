# Computational Thinking - Termos em Inglês e Português com Exemplos

Este guia apresenta os principais conceitos da disciplina de **Pensamento Computacional (Computational Thinking)** em inglês e português, com explicações e exemplos práticos.

---

## Conceitos Fundamentais

- **Computational Thinking** (Pensamento Computacional): Processo de formular problemas e suas soluções de maneira que possam ser representados e resolvidos por um computador.

- **Algorithm** (Algoritmo): Sequência finita de instruções para resolver um problema.

- **Decomposition** (Decomposição): Divisão de um problema complexo em partes menores e mais fáceis de resolver.

- **Pattern Recognition** (Reconhecimento de Padrões): Identificação de semelhanças ou regularidades em dados ou problemas.

- **Abstraction** (Abstração): Foco nos aspectos importantes de um problema, ignorando detalhes irrelevantes.

- **Automation** (Automação): Execução de processos de forma automática por máquinas ou programas.

---

## Estruturas de Controle

- **Sequence** (Sequência): Execução de instruções em ordem.

  ```python
  print("Passo 1: Acenda o fogão")
  print("Passo 2: Coloque a panela")
  print("Passo 3: Aqueça a água")
  ```

**Selection** (Seleção): Estrutura de decisão, escolha de caminhos diferentes.

  ```python
  x = 10
  if x > 5:
      print("x é maior que 5")
  else:
      print("x é menor ou igual a 5")
  ```

**Iteration** (Iteração / Repetição): Execução repetida de instruções.

  ```python
  for i in range(5):
      print("Repetição número", i)
  ```

---

## Estruturas de Dados Básicas

**Variable** (Variável): Espaço nomeado para armazenar valores.

  ```python
  idade = 20
  nome = "Ana"
  ```

**List / Array** (Lista / Vetor): Conjunto ordenado de elementos.

  ```python
  numeros = [1, 2, 3, 4, 5]
  ```

**Dictionary** (Dicionário): Conjunto de pares chave-valor.

  ```python
  aluno = {"nome": "Carlos", "idade": 21}
  ```

---

## Pensamento Algorítmico

**Flowchart** (Fluxograma): Representação gráfica de um algoritmo.

**Pseudocode** (Pseudocódigo): Representação textual simples de um algoritmo, próxima da linguagem natural.

  Exemplo de pseudocódigo:

  ```bash
  INÍCIO
      Leia nota1, nota2
      média = (nota1 + nota2) / 2
      Se média >= 7 então
          Escreva "Aprovado"
      Senão
          Escreva "Reprovado"
  FIM
  ```

---

## Resolução de Problemas

**Problem Solving** (Resolução de Problemas): Capacidade de aplicar estratégias para encontrar soluções computacionais.

**Debugging** (Depuração): Processo de encontrar e corrigir erros em programas.

  ```python
  # Código com erro
  x = 5
  y = 0
  # print(x / y)  # Erro de divisão por zero

  # Correção
  if y != 0:
      print(x / y)
  else:
      print("Não é possível dividir por zero")
  ```

---

## Aplicações Educacionais

**Scratch** (Scratch): Linguagem de programação visual usada para introdução ao pensamento computacional.

**Python** (Python): Linguagem de programação amplamente usada para ensinar fundamentos computacionais.

**Block-based Programming** (Programação em Blocos): Abordagem visual que facilita o aprendizado inicial.

---
