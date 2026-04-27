#!/bin/bash
# info_cpu.sh — coleta info da CPU formatada para colar no relatório
# Aula 1 - Infraestrutura de Hardware
# Prof. Ronierison Maciel

set -e

OUT_FILE="/tmp/info_cpu_$(date +%Y%m%d_%H%M%S).txt"

{
echo "============================================================"
echo "RELATÓRIO DE CPU - $(date)"
echo "Hostname: $(hostname)"
echo "============================================================"
echo ""
echo "## lscpu (resumido)"
echo ""
lscpu | grep -E "Model name|Architecture|CPU\(s\)|Thread\(s\)|Core\(s\)|Socket\(s\)|CPU max MHz|CPU min MHz|L1d cache|L1i cache|L2 cache|L3 cache|Flags" \
  | sed 's/^/    /'
echo ""
echo "## Frequência atual por core"
echo ""
grep "cpu MHz" /proc/cpuinfo | awk '{printf "    Core %d: %s MHz\n", NR-1, $4}'
echo ""
echo "## Topologia (lstopo, modo texto)"
echo ""
if command -v lstopo &> /dev/null; then
  lstopo --of console 2>/dev/null | sed 's/^/    /'
else
  echo "    lstopo não instalado. Instale com: sudo apt install hwloc"
fi
echo ""
echo "## Flags de SIMD relevantes"
echo ""
FLAGS=$(grep -m1 "flags" /proc/cpuinfo)
for flag in sse sse2 sse4_1 sse4_2 avx avx2 avx512f fma; do
  if echo "$FLAGS" | grep -qw "$flag"; then
    echo "    [✓] $flag"
  else
    echo "    [ ] $flag"
  fi
done
echo ""
echo "============================================================"
echo "Saída salva em: $OUT_FILE"
echo "============================================================"
} | tee "$OUT_FILE"
