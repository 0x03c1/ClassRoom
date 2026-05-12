#!/usr/bin/env bash
# Script universal: detecta o sistema e chama o script correto.

set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

case "$(uname -s)" in
    Darwin*)  bash "$SCRIPT_DIR/compile_macos.sh" ;;
    Linux*)   bash "$SCRIPT_DIR/compile_linux.sh" ;;
    *)        echo "Sistema nao suportado por este script. Use compile.bat no Windows." ;;
esac
