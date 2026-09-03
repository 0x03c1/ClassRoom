#!/bin/sh
# Uso: ./inferir.sh 01_latch.v
# Mostra QUAL elemento de memoria a ferramenta inferiu a partir do seu codigo.
echo "=== o que o Yosys inferiu de $1 ==="
yosys -p "read_verilog $1; proc; opt; stat" 2>/dev/null | sed -n '/=== /,/^End/p' | grep -E '[$]|Number of cells'
echo
echo "=== o que o Verilator reclama de $1 ==="
verilator --lint-only -Wall -Wno-DECLFILENAME $1 2>&1 | grep -E 'LATCH|WIDTH|CASEINCOMPLETE|BLKSEQ' | head -6
echo "(nada acima = nenhum aviso)"
