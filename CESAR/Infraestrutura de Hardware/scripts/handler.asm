# handler.asm
# Exemplo simples de exceção + handler no MARS (MIPS32)

.data
exc_code:      .word 0
msg_inicio:    .asciiz "Iniciando teste de excecao...\n"
msg_handler:   .asciiz "Excecao tratada pelo handler.\n"
msg_continua:  .asciiz "Programa continuou apos eret.\n"

.text
.globl main
main:
    # Mensagem inicial
    li   $v0, 4
    la   $a0, msg_inicio
    syscall

    # Gera overflow (excecao codigo 12)
    li   $t0, 0x7fffffff
    addi $t0, $t0, 1        # overflow intencional

    # Execucao continua aqui apos o handler (eret)
    lw   $t1, exc_code
    li   $t2, 12
    bne  $t1, $t2, fim

    li   $v0, 4
    la   $a0, msg_handler
    syscall

    li   $v0, 4
    la   $a0, msg_continua
    syscall

fim:
    li   $v0, 10
    syscall


# Vetor de excecao do MIPS no MARS
.ktext 0x80000180
handler:
    # Le codigo da excecao: Cause[6..2]
    mfc0 $k0, $13
    srl  $k0, $k0, 2
    andi $k0, $k0, 0x1F

    # Salva codigo para o programa principal
    la   $k1, exc_code
    sw   $k0, 0($k1)

    # Avanca EPC para pular a instrucao que causou excecao
    mfc0 $k1, $14
    addiu $k1, $k1, 4
    mtc0 $k1, $14

    eret
