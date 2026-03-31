; ============================================================
; soma_vetor.asm — Soma de inteiros de 1 até N
; Disciplina: Infraestrutura de Hardware (3º período)
; Montador: NASM | Plataforma: Linux x86-64
;
; O QUE ESTE CÓDIGO ENSINA:
;   - O processador executa instruções diretamente em registradores
;   - Não há interpretador, não há runtime, não há GC
;   - Cada instrução mapeia 1:1 para opcodes da CPU
;
; COMPILAR E EXECUTAR:
;   nasm -f elf64 soma_vetor.asm -o soma_vetor.o
;   ld soma_vetor.o -o soma_vetor
;   time ./soma_vetor
; ============================================================

section .data
    msg_resultado db "Soma = ", 0
    newline       db 10              ; '\n'

section .bss
    buffer resb 20                   ; buffer para converter int -> string

section .text
    global _start

_start:
    ; -----------------------------------------------
    ; FASE 1: Calcular soma de 1 até N (N = 1 bilhão)
    ; Usa apenas registradores — zero acesso à RAM
    ; -----------------------------------------------
    xor     rax, rax            ; rax = acumulador = 0
    mov     rcx, 1000000000     ; rcx = contador = 1.000.000.000
    ; Loop de soma: cada iteração = ADD + DEC + JNZ (3 instruções)
.loop:
    add     rax, rcx            ; acumulador += contador
    dec     rcx                 ; contador--
    jnz     .loop               ; se contador != 0, repete

    ; rax agora contém a soma: 500000000500000000
    ; -----------------------------------------------
    ; FASE 2: Converter inteiro para string ASCII
    ; (syscall write precisa de string, não de inteiro)
    ; -----------------------------------------------
    mov     rdi, buffer + 19    ; aponta para o fim do buffer
    mov     byte [rdi], 0       ; terminador nulo
    mov     rbx, 10             ; divisor = 10

.convert:
    dec     rdi                 ; move ponteiro para esquerda
    xor     rdx, rdx            ; limpa rdx (parte alta do dividendo)
    div     rbx                 ; rax / 10 → quociente em rax, resto em rdx
    add     dl, '0'             ; converte dígito para ASCII
    mov     [rdi], dl           ; armazena caractere
    test    rax, rax            ; quociente == 0?
    jnz     .convert            ; se não, continua

    ; rdi agora aponta para o início da string numérica
    ; -----------------------------------------------
    ; FASE 3: Imprimir resultado via syscall
    ; -----------------------------------------------
    ; Imprime "Soma = "
    mov     rax, 1              ; syscall write
    mov     rsi, msg_resultado  ; ponteiro para a mensagem
    mov     rdx, 7              ; tamanho da mensagem
    push    rdi                 ; salva ponteiro do número
    mov     rdi, 1              ; stdout
    syscall

    ; Imprime o número
    pop     rsi                 ; recupera ponteiro do número
    mov     rax, 1              ; syscall write
    mov     rdi, 1              ; stdout
    ; Calcula tamanho da string numérica
    lea     rdx, [buffer + 19]
    sub     rdx, rsi            ; rdx = comprimento
    syscall

    ; Imprime newline
    mov     rax, 1
    mov     rdi, 1
    mov     rsi, newline
    mov     rdx, 1
    syscall

    ; -----------------------------------------------
    ; FASE 4: Sair limpo
    ; -----------------------------------------------
    mov     rax, 60             ; syscall exit
    xor     rdi, rdi            ; exit code 0
    syscall
