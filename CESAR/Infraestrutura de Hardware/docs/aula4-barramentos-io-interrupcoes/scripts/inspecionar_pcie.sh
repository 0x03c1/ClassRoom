#!/bin/bash
# inspecionar_pcie.sh — coleta info de PCIe formatada para o relatório
# Aula 4 - Infraestrutura de Hardware
# Prof. Ronierison Maciel

set -e

if [[ $EUID -ne 0 ]]; then
   echo "Este script precisa de sudo para acessar detalhes do PCIe."
   echo "Execute: sudo $0"
   exit 1
fi

echo "============================================================"
echo "INSPEÇÃO DE BARRAMENTO PCIe - $(date)"
echo "============================================================"
echo ""

echo "## Dispositivos NVMe"
echo ""
NVME_DEVS=$(lspci | grep -i nvme | awk '{print $1}')
if [[ -z "$NVME_DEVS" ]]; then
    echo "    Nenhum NVMe detectado."
else
    for dev in $NVME_DEVS; do
        echo "  Dispositivo: $dev"
        lspci -vv -s "$dev" 2>/dev/null | grep -E "LnkCap:|LnkSta:" | sed 's/^/    /'
        echo ""
    done
fi

echo "## GPUs"
echo ""
GPU_DEVS=$(lspci | grep -iE "vga|3d controller" | awk '{print $1}')
if [[ -z "$GPU_DEVS" ]]; then
    echo "    Nenhuma GPU dedicada detectada."
else
    for dev in $GPU_DEVS; do
        echo "  Dispositivo: $dev"
        lspci -vv -s "$dev" 2>/dev/null | grep -E "LnkCap:|LnkSta:" | sed 's/^/    /'
        echo ""
    done
fi

echo "## Controladores de Rede"
echo ""
NET_DEVS=$(lspci | grep -iE "ethernet|network" | awk '{print $1}')
for dev in $NET_DEVS; do
    echo "  Dispositivo: $dev"
    lspci -vv -s "$dev" 2>/dev/null | grep -E "LnkCap:|LnkSta:" | sed 's/^/    /'
    echo ""
done

echo "============================================================"
echo "Como ler:"
echo "  LnkCap = capacidade máxima do slot/dispositivo"
echo "  LnkSta = o que está SENDO USADO no momento"
echo "  Speed 8GT/s  = PCIe 3.0    (~985 MB/s por lane)"
echo "  Speed 16GT/s = PCIe 4.0    (~1969 MB/s por lane)"
echo "  Speed 32GT/s = PCIe 5.0    (~3938 MB/s por lane)"
echo ""
echo "Se LnkSta < LnkCap, o dispositivo está rodando ABAIXO do potencial!"
echo "============================================================"
