#!/bin/bash
# info_cpu.sh — coleta info da CPU formatada para colar no relatório
# Aula 1 - Infraestrutura de Hardware
# Prof. Roni Maciel
#
# Funciona em x86_64 e aarch64 (ARM). No macOS, use diretamente:
#   sysctl -a | grep machdep.cpu

set -e

OUT_FILE="/tmp/info_cpu_$(date +%Y%m%d_%H%M%S).txt"
ARCH=$(uname -m)

{
echo "============================================================"
echo "RELATÓRIO DE CPU - $(date)"
echo "Hostname: $(hostname)"
echo "Arquitetura: $ARCH"
echo "============================================================"
echo ""
echo "## lscpu (resumido)"
echo ""
lscpu | grep -E "Model name|Architecture|CPU\(s\)|Thread\(s\)|Core\(s\)|Socket\(s\)|CPU max MHz|CPU min MHz|L1d cache|L1i cache|L2 cache|L3 cache|Flags|Features" \
  | sed 's/^/    /'
echo ""

echo "## Frequência atual por core"
echo ""
if grep -q "cpu MHz" /proc/cpuinfo 2>/dev/null; then
    # x86 Linux clássico
    grep "cpu MHz" /proc/cpuinfo | awk '{printf "    Core %d: %s MHz\n", NR-1, $4}'
elif ls /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq &>/dev/null; then
    # ARM Linux ou x86 sem 'cpu MHz' no /proc/cpuinfo (alguns kernels novos)
    for f in /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq; do
        cpu=$(basename "$(dirname "$(dirname "$f")")")
        khz=$(cat "$f")
        mhz=$(awk "BEGIN { printf \"%.1f\", $khz / 1000 }")
        printf "    %s: %s MHz\n" "$cpu" "$mhz"
    done
else
    echo "    Frequência em tempo real não disponível neste sistema"
    echo "    (comum em containers, VMs e WSL2)"
fi
echo ""

echo "## Topologia (lstopo, modo texto)"
echo ""
if command -v lstopo &> /dev/null; then
  lstopo --of console 2>/dev/null | sed 's/^/    /'
else
  echo "    lstopo não instalado. Instale com: sudo apt install hwloc"
fi
echo ""

echo "## Extensões SIMD/vetoriais relevantes"
echo ""
if [[ "$ARCH" == x86_64 || "$ARCH" == i*86 ]]; then
    FLAGS=$(grep -m1 "flags" /proc/cpuinfo)
    for flag in sse sse2 sse4_1 sse4_2 avx avx2 avx512f fma; do
      if echo "$FLAGS" | grep -qw "$flag"; then
        echo "    [✓] $flag"
      else
        echo "    [ ] $flag"
      fi
    done
elif [[ "$ARCH" == aarch64 || "$ARCH" == arm64 ]]; then
    FLAGS=$(grep -m1 "Features" /proc/cpuinfo 2>/dev/null || echo "")
    for flag in fp asimd sve sve2 crc32 aes sha1 sha2; do
      if echo "$FLAGS" | grep -qw "$flag"; then
        echo "    [✓] $flag"
      else
        echo "    [ ] $flag"
      fi
    done
else
    echo "    Arquitetura $ARCH não tem mapeamento neste script."
    echo "    Inspecione manualmente: cat /proc/cpuinfo"
fi
echo ""
echo "============================================================"
echo "Saída salva em: $OUT_FILE"
echo "============================================================"
} | tee "$OUT_FILE"
