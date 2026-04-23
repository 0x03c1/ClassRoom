# overflow.asm
# Exemplo simples de overflow (inteiro de 32 bits) no MARS

.data
msg1: .asciiz "Valor inicial: 2147483647\n"
msg2: .asciiz "Resultado apos overflow: "
msg3: .asciiz " (decimal)\nHex: 0x"
nl:   .asciiz "\n"

.text
.globl main

main:
    # imprime mensagem inicial
    li   $v0, 4
    la   $a0, msg1
    syscall

    # maior inteiro positivo de 32 bits + 1
    li   $t0, 2147483647     # 0x7FFFFFFF
    li   $t1, 1
    addu $t2, $t0, $t1       # overflow com wrap-around (sem excecao)

    # imprime em decimal
    li   $v0, 4
    la   $a0, msg2
    syscall

    move $a0, $t2
    li   $v0, 1
    syscall

    li   $v0, 4
    la   $a0, msg3
    syscall

    # imprime em hexadecimal (syscall 34 no MARS)
    move $a0, $t2
    li   $v0, 34
    syscall

    li   $v0, 4
    la   $a0, nl
    syscall

    # encerra
    li   $v0, 10
    syscall
