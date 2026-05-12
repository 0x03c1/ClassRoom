#!/usr/bin/env bash
# Script de compilacao para macOS
# Detecta automaticamente Apple Silicon (M1/M2/M3) vs Intel.

set -e

# Detecta o prefixo do Homebrew
if [ -d "/opt/homebrew" ]; then
    BREW_PREFIX="/opt/homebrew"
    echo ">> Detectado Mac Apple Silicon"
else
    BREW_PREFIX="/usr/local"
    echo ">> Detectado Mac Intel"
fi

cd "$(dirname "$0")/.."

echo ">> Compilando exemplo.c..."
gcc exemplo.c -o exemplo \
    -I"$BREW_PREFIX/include" \
    -L"$BREW_PREFIX/lib" \
    -lraylib \
    -framework CoreVideo -framework IOKit -framework Cocoa \
    -framework GLUT -framework OpenGL

echo ">> Compilacao OK. Executando..."
./exemplo
