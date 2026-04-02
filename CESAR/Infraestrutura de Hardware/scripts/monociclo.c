/*
 * ════════════════════════════════════════════════════════════════
 *  SIMULADOR MONOCICLO
 *  Prof. 0x03c1
 *
 *  ISA: subconjunto MIPS de 32 bits
 *  Formato R: [31:26]op [25:21]rs [20:16]rt [15:11]rd [10:6]shamt [5:0]funct
 *  Formato I: [31:26]op [25:21]rs [20:16]rt [15:0]imm16
 *  Formato J: [31:26]op [25:0]target
 *
 *  Instruções:
 *    R-type  funct=0x20  ADD   rd, rs, rt
 *    R-type  funct=0x22  SUB   rd, rs, rt
 *    R-type  funct=0x24  AND   rd, rs, rt
 *    R-type  funct=0x25  OR    rd, rs, rt
 *    R-type  funct=0x2A  SLT   rd, rs, rt
 *    I-type  op=0x23     LW    rt, imm(rs)
 *    I-type  op=0x2B     SW    rt, imm(rs)
 *    I-type  op=0x04     BEQ   rs, rt, offset
 *    J-type  op=0x02     J     target
 *    --      op=0x3F     HALT
 *
 *    RegDst RegWrite ALUSrc Branch MemRead MemWrite MemtoReg ALUOp[1:0] Jump
 * ════════════════════════════════════════════════════════════════
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* ── Dimensoes ──────────────────────────────────────────────── */
#define IMEM_WORDS  64
#define DMEM_WORDS  64
#define NUM_REGS    32

/* ── Opcodes ────────────────────────────────────────────────── */
#define OP_RTYPE  0x00
#define OP_LW     0x23
#define OP_SW     0x2B
#define OP_BEQ    0x04
#define OP_J      0x02
#define OP_HALT   0x3F

/* ── Funct R-type ───────────────────────────────────────────── */
#define FUNCT_ADD  0x20
#define FUNCT_SUB  0x22
#define FUNCT_AND  0x24
#define FUNCT_OR   0x25
#define FUNCT_SLT  0x2A

/* ── ALUOp de 2 bits ─────────────────────────────────────────── */
#define ALUOP_ADD    0   /* lw/sw: calcula endereco            */
#define ALUOP_SUB    1   /* beq: compara ($rs - $rt == 0?)     */
#define ALUOP_RTYPE  2   /* R-type: decodifica funct           */

/* ── Cores ANSI ─────────────────────────────────────────────── */
#define RST  "\033[0m"
#define BOLD "\033[1m"
#define DIM  "\033[2m"
#define CYA  "\033[36m"
#define YEL  "\033[33m"
#define GRN  "\033[32m"
#define RED  "\033[31m"
#define MAG  "\033[35m"
#define BLU  "\033[34m"

/* ════════════════════════════════════════════════════════════════
 *  ESTADO DO PROCESSADOR
 * ════════════════════════════════════════════════════════════════ */
typedef struct {
    uint32_t PC;
    uint32_t IMEM[IMEM_WORDS];
    int32_t  DMEM[DMEM_WORDS];
    int32_t  RF[NUM_REGS];
    int      cycle;
    /* contadores para relatorio de desempenho */
    int      n_rtype, n_lw, n_sw, n_beq, n_j;
} CPU;

/* ── Sinais de controle ── */
typedef struct {
    int RegDst;   /* 1=rd (R-type)  0=rt (I-type)              */
    int RegWrite; /* 1=escreve no banco                        */
    int ALUSrc;   /* 0=operando2 e reg  1=imediato             */
    int Branch;   /* 1=instrucao de desvio condicional         */
    int MemRead;  /* 1=le DMEM                                 */
    int MemWrite; /* 1=escreve DMEM                            */
    int MemtoReg; /* 0=resultado ULA  1=dado da memoria        */
    int ALUOp;    /* 2 bits: 00=ADD 01=SUB 10=R-type           */
    int Jump;     /* 1=salto incondicional                     */
} CS;

/* ════════════════════════════════════════════════════════════════
 *  CAMPOS DA INSTRUCAO  (Inst[25:21] etc.)
 * ════════════════════════════════════════════════════════════════ */
static inline uint32_t f_op    (uint32_t i){ return (i>>26)&0x3F; }
static inline uint32_t f_rs    (uint32_t i){ return (i>>21)&0x1F; } /* Inst[25:21] */
static inline uint32_t f_rt    (uint32_t i){ return (i>>16)&0x1F; } /* Inst[20:16] */
static inline uint32_t f_rd    (uint32_t i){ return (i>>11)&0x1F; } /* Inst[15:11] */
static inline uint32_t f_funct (uint32_t i){ return  i     &0x3F; }
static inline uint32_t f_imm16 (uint32_t i){ return  i     &0xFFFF; }
static inline uint32_t f_addr26(uint32_t i){ return  i     &0x3FFFFFF; }

/* Sign-Extend 16->32 bits — lw" */
static inline int32_t sign_ext16(uint32_t v){
    return (v & 0x8000) ? (int32_t)(v | 0xFFFF0000) : (int32_t)v;
}

/* ════════════════════════════════════════════════════════════════
 *  UNIDADE DE CONTROLE
 *  Implementa exatamente a tabela dos "Sinais de Controle"
 *
 *  Tabela:
 *  Instrucao | RegDst RegWrite ALUSrc Branch MemRead MemWrite MemtoReg ALUOp Jump
 *  R-type    |   1      1       0      0      0        0        0       10    0
 *  lw        |   0      1       1      0      1        0        1       00    0
 *  sw        |   X      0       1      0      0        1        X       00    0
 *  beq       |   X      0       0      1      0        0        X       01    0
 *  j         |   X      0       X      X      0        0        X       XX    1
 * ════════════════════════════════════════════════════════════════ */
CS unidade_controle(uint32_t op) {
    CS s = {0};
    switch (op) {
        case OP_RTYPE:
            s.RegDst=1; s.RegWrite=1; s.ALUSrc=0;
            s.Branch=0; s.MemRead=0;  s.MemWrite=0;
            s.MemtoReg=0; s.ALUOp=ALUOP_RTYPE; s.Jump=0;
            break;
        case OP_LW:
            s.RegDst=0; s.RegWrite=1; s.ALUSrc=1;
            s.Branch=0; s.MemRead=1;  s.MemWrite=0;
            s.MemtoReg=1; s.ALUOp=ALUOP_ADD; s.Jump=0;
            break;
        case OP_SW:
            s.RegDst=0; s.RegWrite=0; s.ALUSrc=1;
            s.Branch=0; s.MemRead=0;  s.MemWrite=1;
            s.MemtoReg=0; s.ALUOp=ALUOP_ADD; s.Jump=0;
            break;
        case OP_BEQ:
            s.RegDst=0; s.RegWrite=0; s.ALUSrc=0;
            s.Branch=1; s.MemRead=0;  s.MemWrite=0;
            s.MemtoReg=0; s.ALUOp=ALUOP_SUB; s.Jump=0;
            break;
        case OP_J:
            s.RegDst=0; s.RegWrite=0; s.ALUSrc=0;
            s.Branch=0; s.MemRead=0;  s.MemWrite=0;
            s.MemtoReg=0; s.ALUOp=ALUOP_ADD; s.Jump=1;
            break;
    }
    return s;
}

/* ════════════════════════════════════════════════════════════════
 *  CONTROLE DA ULA — decodifica ALUOp + funct
 * ════════════════════════════════════════════════════════════════ */
typedef enum { ULA_ADD, ULA_SUB, ULA_AND, ULA_OR, ULA_SLT } ULAOp;

ULAOp ula_control(int aluop, uint32_t funct) {
    if (aluop == ALUOP_ADD) return ULA_ADD;
    if (aluop == ALUOP_SUB) return ULA_SUB;
    switch (funct) {
        case FUNCT_ADD: return ULA_ADD;
        case FUNCT_SUB: return ULA_SUB;
        case FUNCT_AND: return ULA_AND;
        case FUNCT_OR:  return ULA_OR;
        case FUNCT_SLT: return ULA_SLT;
        default:        return ULA_ADD;
    }
}

const char *ulaop_nome[] = {"ADD","SUB","AND","OR ","SLT"};

/* ════════════════════════════════════════════════════════════════
 *  UTILITARIOS DE EXIBICAO
 * ════════════════════════════════════════════════════════════════ */
void sep(const char *cor, char c, int n) {
    printf("%s", cor);
    for (int i=0; i<n; i++) putchar(c);
    printf("%s\n", RST);
}

void show_mux(const char *nome, int sel,
              const char *in0, const char *in1, const char *out) {
    printf(BOLD BLU "  [MUX %-20s" RST
           " sel=%d  in0=%s%-16s%s  in1=%s%-16s%s  "
           BOLD "-> %s\n" RST,
           nome, sel,
           sel==0 ? GRN : DIM, in0, RST,
           sel==1 ? GRN : DIM, in1, RST,
           out);
}

void show_ula(ULAOp op, int32_t A, int32_t B, int32_t res, int zero) {
    printf(BOLD RED "  [ULA %-3s" RST
           "  A=%-8d  B=%-8d  " BOLD "resultado=%-8d" RST "  zero=%d\n",
           ulaop_nome[op], A, B, res, zero);
}

/* ════════════════════════════════════════════════════════════════
 *  NOMES DOS REGISTRADORES MIPS
 * ════════════════════════════════════════════════════════════════ */
const char *regname[32] = {
    "$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3",
    "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
    "$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
    "$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"
};

const char *instr_nome(uint32_t instr) {
    uint32_t op = f_op(instr);
    if (op == OP_RTYPE) {
        switch (f_funct(instr)) {
            case FUNCT_ADD: return "ADD";
            case FUNCT_SUB: return "SUB";
            case FUNCT_AND: return "AND";
            case FUNCT_OR:  return "OR ";
            case FUNCT_SLT: return "SLT";
            default:        return "R? ";
        }
    }
    if (op==OP_LW)   return "LW ";
    if (op==OP_SW)   return "SW ";
    if (op==OP_BEQ)  return "BEQ";
    if (op==OP_J)    return "J  ";
    if (op==OP_HALT) return "HLT";
    return "???";
}

/* ════════════════════════════════════════════════════════════════
 *  DUMP DO ESTADO — snapshot entre ciclos
 * ════════════════════════════════════════════════════════════════ */
void dump_state(const CPU *cpu) {
    uint32_t wi = cpu->PC / 4;
    printf("\n");
    sep(CYA,'=',70);
    printf(BOLD CYA "  CICLO %d" RST
           "  |  PC=0x%08X (IMEM[%u])"
           "  |  proxima instrucao: %s\n",
           cpu->cycle, cpu->PC, wi,
           wi < IMEM_WORDS ? instr_nome(cpu->IMEM[wi]) : "---");
    sep(CYA,'=',70);

    /* Banco de Registradores — mostra so nao-zero */
    printf(BOLD GRN "  BANCO DE REGISTRADORES\n" RST);
    sep(GRN,'-',70);
    int cols = 0;
    for (int i=0; i<NUM_REGS; i++) {
        if (cpu->RF[i] != 0 || i==0) {
            printf("  %-6s= %8d (0x%08X)  ",
                   regname[i], cpu->RF[i], (uint32_t)cpu->RF[i]);
            if (++cols % 3 == 0) printf("\n");
        }
    }
    if (cols % 3 != 0) printf("\n");

    /* Memoria de Dados — primeiras 8 palavras */
    sep(MAG,'-',70);
    printf(BOLD MAG "  MEMORIA DE DADOS  (palavras 0..7)\n" RST);
    for (int i=0; i<8; i++) {
        printf("  DMEM[%d]=%-10d", i, cpu->DMEM[i]);
        if ((i+1)%4==0) printf("\n");
    }
    if (8%4 != 0) printf("\n");
    sep(CYA,'=',70);
}

/* ════════════════════════════════════════════════════════════════
 *  EXECUCAO DE UM CICLO MONOCICLO COMPLETO
 * ════════════════════════════════════════════════════════════════ */
int execute_cycle(CPU *cpu) {
    uint32_t wi = cpu->PC / 4;
    if (wi >= IMEM_WORDS) return -1;

    uint32_t instr  = cpu->IMEM[wi];
    uint32_t op     = f_op(instr);
    if (op == OP_HALT) return -1;

    cpu->cycle++;

    uint32_t r_rs   = f_rs(instr);     /* Inst[25:21] */
    uint32_t r_rt   = f_rt(instr);     /* Inst[20:16] */
    uint32_t r_rd   = f_rd(instr);     /* Inst[15:11] */
    uint32_t funct  = f_funct(instr);
    int32_t  imm16  = sign_ext16(f_imm16(instr));
    uint32_t addr26 = f_addr26(instr);

    /* ── FETCH ──────────────────────────────────────────────────
     * PC fornece endereco → Mem.Inst retorna instrucao 32 bits
     * Adder PC+4 calcula proximo endereco sequencial
     * ─────────────────────────────────────────────────────────── */
    sep(YEL,'-',70);
    printf(BOLD YEL "  ESTAGIO: FETCH" RST
           "  [Passo 1]\n");
    printf("  PC=0x%08X → IMEM[%u] = 0x%08X  (%s)\n",
           cpu->PC, wi, instr, instr_nome(instr));
    printf("  Adder PC+4: 0x%08X + 4 = 0x%08X\n",
           cpu->PC, cpu->PC+4);
    if (op == OP_RTYPE)
        printf("  Campos R: op=0x%02X  rs=%s  rt=%s  rd=%s"
               "  shamt=%u  funct=0x%02X\n",
               op, regname[r_rs], regname[r_rt], regname[r_rd],
               (instr>>6)&0x1F, funct);
    else if (op == OP_J)
        printf("  Campos J: op=0x%02X  target=0x%07X\n", op, addr26);
    else
        printf("  Campos I: op=0x%02X  rs=%s  rt=%s"
               "  imm16=0x%04X (sem sinal=%u  sign-ext=%d)\n",
               op, regname[r_rs], regname[r_rt],
               f_imm16(instr), f_imm16(instr), imm16);

    /* ── DECODE + UNIDADE DE CONTROLE ──────────────────────────
     * opcode → Unidade de Controle → todos os sinais
     * Banco le Read Data 1 (rs) e Read Data 2 (rt)
     * Sign-Extend recebe Inst[15:0] → imm32
     * ─────────────────────────────────────────────────────────── */
    sep(YEL,'-',70);
    printf(BOLD YEL "  ESTAGIO: DECODE + UNIDADE DE CONTROLE" RST
           "  [Passo 2, Sinais de Controle]\n");

    CS cs = unidade_controle(op);

    const char *tipo_str;
    if (op==OP_RTYPE) tipo_str="R-type";
    else if(op==OP_LW)tipo_str="lw    ";
    else if(op==OP_SW)tipo_str="sw    ";
    else if(op==OP_BEQ)tipo_str="beq   ";
    else if(op==OP_J) tipo_str="j     ";
    else               tipo_str="?     ";

    printf("  Tipo=%-6s  RegDst=%d  RegWrite=%d  ALUSrc=%d  Branch=%d  "
           "MemRead=%d  MemWrite=%d  MemtoReg=%d  ALUOp=%02d  Jump=%d\n",
           tipo_str,
           cs.RegDst, cs.RegWrite, cs.ALUSrc, cs.Branch,
           cs.MemRead, cs.MemWrite, cs.MemtoReg,
           cs.ALUOp, cs.Jump);

    int32_t ReadData1 = cpu->RF[r_rs];
    int32_t ReadData2 = cpu->RF[r_rt];
    printf("  Banco Reg → Read Data 1 (%s) = %d\n", regname[r_rs], ReadData1);
    printf("  Banco Reg → Read Data 2 (%s) = %d\n", regname[r_rt], ReadData2);
    printf("  Sign-Extend Inst[15:0]=0x%04X → %d\n",
           f_imm16(instr), imm16);

    /* ── EXECUTE — ULA ──────────────────────────────────────────
     * MUX ALUSrc: Read Data 2 (ALUSrc=0) ou imm32 (ALUSrc=1)
     * Controle ULA: ALUOp + funct → operacao
     * Flag Zero → usada pelo BEQ
     * Somador de branch: PC+4 + (imm16 << 2)  [componente separado]
     * ─────────────────────────────────────────────────────────── */
    sep(YEL,'-',70);
    printf(BOLD YEL "  ESTAGIO: EXECUTE — ULA" RST
           "  [Passo 3, beq]\n");

    char s0[40], s1[40], ss[40];
    snprintf(s0,40,"%s=%d",  regname[r_rt], ReadData2);
    snprintf(s1,40,"imm=%d", imm16);
    int32_t ula_B = cs.ALUSrc ? imm16 : ReadData2;
    snprintf(ss,40,"%d", ula_B);
    show_mux("ALUSrc]", cs.ALUSrc, s0, s1, ss);

    ULAOp uop = ula_control(cs.ALUOp, funct);
    int32_t ula_res = 0;
    switch (uop) {
        case ULA_ADD: ula_res = ReadData1 + ula_B; break;
        case ULA_SUB: ula_res = ReadData1 - ula_B; break;
        case ULA_AND: ula_res = ReadData1 & ula_B; break;
        case ULA_OR:  ula_res = ReadData1 | ula_B; break;
        case ULA_SLT: ula_res = (ReadData1 < ula_B) ? 1 : 0; break;
    }
    int zero = (ula_res == 0);
    show_ula(uop, ReadData1, ula_B, ula_res, zero);

    /* Somador de branch ("Adicionando beq") */
    int32_t branch_tgt = (int32_t)(cpu->PC + 4) + (imm16 << 2);
    printf("  Somador Branch: PC+4 + (imm16<<2) = %u + %d = %d\n",
           cpu->PC+4, imm16<<2, branch_tgt);

    /* ── MEMORY ─────────────────────────────────────────────────
     * "Datapath com R-type e lw" + "Adicionando sw"
     * MemRead=1  → leitura: Read Data = DMEM[ula_res/4]
     * MemWrite=1 → escrita: DMEM[ula_res/4] ← Read Data 2
     * ─────────────────────────────────────────────────────────── */
    int32_t MemReadData = 0;
    if (cs.MemRead || cs.MemWrite) {
        sep(YEL,'-',70);
        printf(BOLD YEL "  ESTAGIO: MEMORY" RST
               "  [lw, sw]\n");
        int waddr = ula_res / 4;
        if (waddr < 0 || waddr >= DMEM_WORDS) {
            printf(RED "  ERRO: endereco %d (palavra %d) fora da DMEM!\n" RST,
                   ula_res, waddr);
        } else if (cs.MemRead) {
            MemReadData = cpu->DMEM[waddr];
            printf("  MemRead=1  DMEM[%d] = %d  → Read Data\n",
                   waddr, MemReadData);
        } else {
            cpu->DMEM[waddr] = ReadData2;
            printf("  MemWrite=1  DMEM[%d] <- %d  (= %s)\n",
                   waddr, ReadData2, regname[r_rt]);
        }
    }

    /* ── WRITE-BACK ─────────────────────────────────────────────
     * "Passo 2", "Datapath Completo"
     * MUX RegDst:   destino = rd (R-type) ou rt (I-type)
     * MUX MemtoReg: dado = resultado ULA (0) ou leitura mem (1)
     * ─────────────────────────────────────────────────────────── */
    sep(YEL,'-',70);
    printf(BOLD YEL "  ESTAGIO: WRITE-BACK" RST
           "  [Passo 2, Datapath Completo]\n");

    uint32_t dest = cs.RegDst ? r_rd : r_rt;
    snprintf(s0,40,"rt=%s", regname[r_rt]);
    snprintf(s1,40,"rd=%s", regname[r_rd]);
    snprintf(ss,40,"%s",    regname[dest]);
    show_mux("RegDst]", cs.RegDst, s0, s1, ss);

    int32_t wb = cs.MemtoReg ? MemReadData : ula_res;
    snprintf(s0,40,"ULA=%d",   ula_res);
    snprintf(s1,40,"Mem=%d",   MemReadData);
    snprintf(ss,40,"%d",       wb);
    show_mux("MemtoReg]", cs.MemtoReg, s0, s1, ss);

    if (cs.RegWrite && dest != 0) {
        cpu->RF[dest] = wb;
        printf("  Banco Reg <- %s = %d\n", regname[dest], wb);
    } else if (!cs.RegWrite) {
        printf("  RegWrite=0  (sem escrita no banco)\n");
    } else {
        printf("  $zero e somente leitura — escrita ignorada\n");
    }

    /* ── PROXIMO PC ─────────────────────────────────────────────
     * "Adicionando beq" e "Adicionando j"
     * Porta AND: PCSrc = Branch AND Zero
     * MUX PCSrc: PC+4 ou Branch Target
     * MUX Jump:  saida PCSrc ou endereco de jump
     * Jump addr = PC[31:28] || Inst[25:0]<<2
     * ─────────────────────────────────────────────────────────── */
    sep(YEL,'-',70);
    printf(BOLD YEL "  ESTAGIO: PROXIMO PC" RST
           "  [beq, j]\n");

    uint32_t pc4  = cpu->PC + 4;
    int PCSrc     = cs.Branch & zero;     /* porta AND */

    snprintf(s0,40,"PC+4=%u",       pc4);
    snprintf(s1,40,"branch=%d",     branch_tgt);
    snprintf(ss,40,"%u", PCSrc ? (uint32_t)branch_tgt : pc4);
    show_mux("PCSrc (Branch AND Zero)]", PCSrc, s0, s1, ss);
    printf("  Branch=%d  Zero=%d  PCSrc = Branch AND Zero = %d\n",
           cs.Branch, zero, PCSrc);

    uint32_t next_pc = PCSrc ? (uint32_t)branch_tgt : pc4;

    /* Jump — "Adicionando j": PC[31:28] || addr26<<2 */
    uint32_t jump_addr = (pc4 & 0xF0000000) | (addr26 << 2);
    snprintf(s0,40,"PCSrc_out=%u",  next_pc);
    snprintf(s1,40,"jump=0x%08X",   jump_addr);
    snprintf(ss,40,"0x%08X", cs.Jump ? jump_addr : next_pc);
    show_mux("Jump]", cs.Jump, s0, s1, ss);

    if (cs.Jump) {
        printf("  Jump: PC[31:28]=0x%X || Inst[25:0]<<2=0x%X → 0x%08X\n",
               (pc4>>28)&0xF, addr26<<2, jump_addr);
    }

    cpu->PC = cs.Jump ? jump_addr : next_pc;
    printf("  PC <- 0x%08X\n", cpu->PC);

    /* contadores de desempenho */
    switch (op) {
        case OP_RTYPE: cpu->n_rtype++; break;
        case OP_LW:    cpu->n_lw++;    break;
        case OP_SW:    cpu->n_sw++;    break;
        case OP_BEQ:   cpu->n_beq++;   break;
        case OP_J:     cpu->n_j++;     break;
    }
    return 0;
}

/* ════════════════════════════════════════════════════════════════
 *  RELATORIO DE DESEMPENHO
 *  "Desempenho: CPI, Clock e Tempo de Execucao"
 *  T_exec = N_inst x CPI x T_clock   (CPI = 1 no monociclo!)
 * ════════════════════════════════════════════════════════════════ */
void perf_report(const CPU *cpu) {
    int total = cpu->n_rtype + cpu->n_lw + cpu->n_sw
              + cpu->n_beq + cpu->n_j;

    /* Latencias tipicas em ps (Patterson & Hennessy) */
    int lat_rtype=600, lat_lw=800, lat_sw=700,
        lat_beq=500, lat_j=200;
    /* T_clock monociclo = instrucao mais lenta = lw = 800 ps   */
    int T_clock = 800;

    printf("\n");
    sep(CYA,'=',70);
    printf(BOLD CYA "  RELATORIO DE DESEMPENHO" RST
           "  [CPI, Clock e Tempo de Execucao]\n");
    sep(CYA,'=',70);
    printf("  Instrucoes executadas:\n");
    printf("    R-type : %3d  (latencia individual = %d ps)\n",
           cpu->n_rtype, lat_rtype);
    printf("    lw     : %3d  (latencia individual = %d ps)\n",
           cpu->n_lw,    lat_lw);
    printf("    sw     : %3d  (latencia individual = %d ps)\n",
           cpu->n_sw,    lat_sw);
    printf("    beq    : %3d  (latencia individual = %d ps)\n",
           cpu->n_beq,   lat_beq);
    printf("    j      : %3d  (latencia individual = %d ps)\n",
           cpu->n_j,     lat_j);
    printf("    TOTAL  : %3d instrucao(oes)\n\n", total);

    printf("  CPI = 1  (monociclo: sempre 1 instrucao por ciclo)\n");
    printf("  T_clock = %d ps  (limitado pela lw — instrucao mais lenta!)\n\n",
           T_clock);

    long long t_exec = (long long)total * 1 * T_clock;
    printf("  Calculo:\n");
    printf("    T_exec = N_inst x CPI x T_clock\n");
    printf("           = %d x 1 x %d ps\n", total, T_clock);
    printf("           = " BOLD "%lld ps  =  %.3f ns\n" RST "\n",
           t_exec, t_exec/1000.0);

    printf("  Se fosse multiciclo, a lw usaria 800 ps mas\n");
    printf("  as R-type usariam apenas 600 ps — T_clock seria menor!\n");
    sep(CYA,'=',70);
}

/* ════════════════════════════════════════════════════════════════
 *  PROGRAMA DE DEMONSTRACAO
 *
 *  Assembly equivalente (MIPS 32 bits):
 *    # Dados: $t0=10, $t1=7, DMEM[0]=42
 *    add  $t2, $t0, $t1     # $t2 = 10+7 = 17     (R-type)
 *    sub  $t3, $t0, $t1     # $t3 = 10-7 = 3      (R-type)
 *    and  $t4, $t0, $t1     # $t4 = 10&7 = 2      (R-type)
 *    or   $t5, $t0, $t1     # $t5 = 10|7 = 15     (R-type)
 *    slt  $t6, $t1, $t0     # $t6 = 1             (R-type)
 *    sw   $t2, 4($zero)     # DMEM[1] <- 17       (sw)
 *    lw   $s0, 0($zero)     # $s0 = DMEM[0] = 42  (lw)
 *    beq  $t0, $t1, 2       # 10!=7 → NAO desvia  (beq)
 *    beq  $t3, $t3, 1       # 3==3  → DESVIA!     (beq)
 *    add  $t7, $zero, $zero # PULADO pelo branch
 *    halt
 * ════════════════════════════════════════════════════════════════ */
#define RTYPE(rs,rt,rd,fn) ((OP_RTYPE<<26)|((rs)<<21)|((rt)<<16)|((rd)<<11)|(fn))
#define ITYPE(op,rs,rt,im) (((op)<<26)|((rs)<<21)|((rt)<<16)|((im)&0xFFFF))
#define JTYPE(op,tg)       (((op)<<26)|((tg)&0x3FFFFFF))

void load_demo(CPU *cpu) {
    memset(cpu, 0, sizeof(CPU));
    /* $t0=8 $t1=9 $t2=10 $t3=11 $t4=12 $t5=13 $t6=14 $t7=15 $s0=16 */
    cpu->RF[8]  = 10;   /* $t0 = 10 */
    cpu->RF[9]  = 7;    /* $t1 = 7  */
    cpu->DMEM[0] = 42;  /* DMEM[0]  = 42 */

    cpu->IMEM[0] = RTYPE(8, 9,10,FUNCT_ADD); /* add $t2,$t0,$t1  */
    cpu->IMEM[1] = RTYPE(8, 9,11,FUNCT_SUB); /* sub $t3,$t0,$t1  */
    cpu->IMEM[2] = RTYPE(8, 9,12,FUNCT_AND); /* and $t4,$t0,$t1  */
    cpu->IMEM[3] = RTYPE(8, 9,13,FUNCT_OR);  /* or  $t5,$t0,$t1  */
    cpu->IMEM[4] = RTYPE(9, 8,14,FUNCT_SLT); /* slt $t6,$t1,$t0  */
    cpu->IMEM[5] = ITYPE(OP_SW, 0,10, 4);    /* sw  $t2,4($zero) → DMEM[1] */
    cpu->IMEM[6] = ITYPE(OP_LW, 0,16, 0);    /* lw  $s0,0($zero) → $s0=42  */
    cpu->IMEM[7] = ITYPE(OP_BEQ,8, 9, 2);    /* beq $t0,$t1,+2   → nao salta */
    cpu->IMEM[8] = ITYPE(OP_BEQ,11,11, 1);   /* beq $t3,$t3,+1   → SALTA! */
    cpu->IMEM[9] = RTYPE(0, 0,15,FUNCT_ADD); /* add $t7,$0,$0    ← PULADO */
    cpu->IMEM[10]= (OP_HALT<<26);             /* halt */
}

/* ════════════════════════════════════════════════════════════════
 *  MAIN
 * ════════════════════════════════════════════════════════════════ */
int main(void) {
    CPU cpu;
    load_demo(&cpu);

    sep(CYA,'*',70);
    printf(BOLD
           "SIMULADOR MONOCICLO\n"
           RST);
    sep(CYA,'*',70);

    /* Listagem da IMEM (como no MARS/SPIM) */
    printf("\n" BOLD "IMEM — programa carregado:\n" RST);
    for (int i=0; i<IMEM_WORDS; i++) {
        uint32_t ins = cpu.IMEM[i];
        if (ins == 0) break;
        printf("  [0x%04X / IMEM[%2d]]  0x%08X  %-3s",
               i*4, i, ins, instr_nome(ins));
        uint32_t op = f_op(ins);
        if (op==OP_RTYPE)
            printf("  %s, %s, %s",
                   regname[f_rd(ins)], regname[f_rs(ins)], regname[f_rt(ins)]);
        else if (op==OP_LW||op==OP_SW)
            printf("  %s, %d(%s)",
                   regname[f_rt(ins)], sign_ext16(f_imm16(ins)), regname[f_rs(ins)]);
        else if (op==OP_BEQ)
            printf("  %s, %s, %+d",
                   regname[f_rs(ins)], regname[f_rt(ins)], sign_ext16(f_imm16(ins)));
        printf("\n");
    }

    printf("\n" BOLD "Estado inicial:\n" RST);
    dump_state(&cpu);

    printf("\n" DIM
           "  Comandos:\n"
           "    ENTER = executa um ciclo (passo a passo)\n"
           "    r     = executa tudo ate HALT\n"
           "    q     = sair\n" RST "\n");

    char buf[16];
    while (1) {
        printf(BOLD CYA "  [PC=0x%04X | ciclo %d] > " RST,
               cpu.PC, cpu.cycle);
        fflush(stdout);

        if (!fgets(buf, sizeof(buf), stdin)) break;
        char cmd = buf[0];
        if (cmd=='q'||cmd=='Q') break;

        if (cmd=='r'||cmd=='R') {
            int r;
            do { r = execute_cycle(&cpu); } while (r == 0);
            dump_state(&cpu);
            perf_report(&cpu);
            printf(BOLD GRN "\n  *** HALT — execucao concluida ***\n\n" RST);
            break;
        }

        int r = execute_cycle(&cpu);
        dump_state(&cpu);

        if (r < 0) {
            perf_report(&cpu);
            printf(BOLD GRN "\n  *** HALT — execucao concluida ***\n\n" RST);
            break;
        }
    }
    return 0;
}
