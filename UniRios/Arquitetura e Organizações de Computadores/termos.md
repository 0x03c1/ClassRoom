# Computer Architecture and Organization - Termos em Inglês e Português com Exemplos

Este guia apresenta os principais conceitos da disciplina de **Arquitetura e Organização de Computadores (Computer Architecture and Organization)** em inglês e português, com explicações e exemplos práticos.

---

## Conceitos Fundamentais

- **Computer Architecture** (Arquitetura de Computadores): Define o conjunto de instruções, organização e funcionalidades de um sistema computacional.

- **Computer Organization** (Organização de Computadores): Refere-se à forma como os componentes físicos são interconectados e implementados.

- **Instruction Set Architecture (ISA)** (Arquitetura do Conjunto de Instruções): Conjunto de instruções que o processador pode executar.

- **Von Neumann Architecture** (Arquitetura de Von Neumann): Modelo de arquitetura de computadores baseado em unidade de controle, memória, unidade lógica e aritmética (ULA) e dispositivos de entrada/saída.

---

## Componentes de um Computador

- **CPU - Central Processing Unit** (Unidade Central de Processamento): Composta por ULA, unidade de controle e registradores.

- **ALU - Arithmetic Logic Unit** (Unidade Lógica e Aritmética): Executa operações matemáticas e lógicas.

- **Control Unit** (Unidade de Controle): Coordena e controla a execução das instruções.

- **Registers** (Registradores): Memória interna de alta velocidade usada para armazenar dados temporários.

  Exemplo em Assembly (x86):

  ```asm
  MOV AX, 5     ; Armazena o valor 5 no registrador AX
  ADD AX, 3     ; Soma 3 ao valor em AX
  ```

**Cache Memory** (Memória Cache): Memória de alta velocidade que armazena dados frequentemente usados.

**Main Memory (RAM)** (Memória Principal - RAM): Armazena dados e programas em execução.

**Secondary Storage** (Armazenamento Secundário): Discos rígidos, SSDs, entre outros.

---

## Ciclo de Instrução

**Fetch** (Busca): Instrução é buscada na memória.

**Decode** (Decodificação): Instrução é interpretada pela unidade de controle.

**Execute** (Execução): Instrução é executada pela ULA ou outro componente.

**Store** (Armazenamento): Resultado é gravado em registradores ou memória.

---

## Tipos de Instruções

**Data Transfer** (Transferência de Dados): Movimentação de dados entre registradores e memória.

  ```asm
  MOV AX, [1000h]   ; Move o valor da memória para AX
  ```

**Arithmetic** (Aritméticas): Operações matemáticas.

  ```asm
  ADD AX, BX        ; AX = AX + BX
  ```

**Logical** (Lógicas): Operações AND, OR, NOT.

  ```asm
  AND AX, BX
  ```

**Control** (Controle): Instruções de salto ou desvio.

  ```asm
  JMP 2000h
  ```

---

## Organização da Memória

**Memory Hierarchy** (Hierarquia de Memória): Estrutura em níveis que vai do mais rápido e caro (registradores) ao mais lento e barato (disco).

**Cache Levels** (Níveis de Cache): L1, L2, L3 com diferentes capacidades e velocidades.

**Virtual Memory** (Memória Virtual): Técnica que permite expandir a memória principal utilizando o disco.

---

## Sistemas de Entrada e Saída

**I/O Devices** (Dispositivos de Entrada/Saída): Teclados, monitores, impressoras, etc.

**Bus** (Barramento): Sistema de comunicação entre os componentes do computador.

**Interrupt** (Interrupção): Mecanismo que altera o fluxo normal da CPU para tratar eventos externos.

---

## Arquiteturas Avançadas

**RISC - Reduced Instruction Set Computer** (Computador com Conjunto Reduzido de Instruções): Processadores com instruções simples e rápidas.

**CISC - Complex Instruction Set Computer** (Computador com Conjunto Complexo de Instruções): Processadores com instruções mais complexas.

**Pipeline** (Pipeline): Técnica para sobrepor etapas do ciclo de instrução, aumentando o desempenho.

**Parallel Architecture** (Arquitetura Paralela): Uso de múltiplos processadores para execução simultânea.

**Distributed Systems** (Sistemas Distribuídos): Conjunto de computadores independentes que trabalham juntos como um único sistema.

---

## Medição de Desempenho

**Clock Cycle** (Ciclo de Clock): Unidade básica de tempo para execução de instruções.

**MIPS - Million Instructions Per Second** (Milhões de Instruções por Segundo): Métrica de desempenho.

**FLOPS - Floating Point Operations Per Second** (Operações de Ponto Flutuante por Segundo): Mede desempenho em cálculos numéricos.

**Benchmark** (Teste de Desempenho): Programa usado para medir e comparar desempenho de sistemas.

---
