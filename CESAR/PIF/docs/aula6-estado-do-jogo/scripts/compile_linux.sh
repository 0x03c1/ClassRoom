#!/usr/bin/env bash
# Script de compilacao para Linux

set -e
cd "$(dirname "$0")/.."

echo ">> Compilando exemplo.c..."
gcc exemplo.c -o exemplo -lraylib -lm

echo ">> Compilacao OK. Executando..."
./exemplo
