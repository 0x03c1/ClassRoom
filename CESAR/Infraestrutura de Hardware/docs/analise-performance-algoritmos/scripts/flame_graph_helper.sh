#!/bin/bash
# flame_graph_helper.sh — Caso 2 (Análise de Performance de Algoritmos)
# Prof. Roni Maciel - Disciplina de Infraestrutura de Hardware
#
# Wrapper que automatiza o fluxo completo de profiling:
#   1. Verifica e instala dependências (FlameGraph, hyperfine)
#   2. Executa hyperfine para benchmark estatístico
#   3. Executa perf record para coletar amostras
#   4. Gera flame graph SVG interativo
#   5. Roda perf stat para diagnóstico arquitetural (cache miss, IPC, etc.)
#
# Uso:
#   ./flame_graph_helper.sh setup                          # instala dependências
#   ./flame_graph_helper.sh bench  "./meu_programa"        # só benchmark estatístico
#   ./flame_graph_helper.sh flame  "./meu_programa"        # só flame graph
#   ./flame_graph_helper.sh stat   "./meu_programa"        # só perf stat
#   ./flame_graph_helper.sh full   "./meu_programa"        # tudo de uma vez
#
# Para Python e Java, use os comandos especializados:
#   ./flame_graph_helper.sh python "python3 script.py"
#   ./flame_graph_helper.sh java   <PID_JAVA>

set -e

FLAMEGRAPH_DIR="${HOME}/FlameGraph"
SAIDA_DIR="$(pwd)/profiling_resultados"
mkdir -p "$SAIDA_DIR"

cor_verde() { echo -e "\033[32m$1\033[0m"; }
cor_amarela() { echo -e "\033[33m$1\033[0m"; }
cor_vermelha() { echo -e "\033[31m$1\033[0m"; }

# ===== SETUP =====

cmd_setup() {
    cor_amarela "▶ Verificando dependências..."

    # FlameGraph
    if [[ ! -d "$FLAMEGRAPH_DIR" ]]; then
        cor_amarela "  Clonando FlameGraph (Brendan Gregg)..."
        git clone --depth=1 https://github.com/brendangregg/FlameGraph.git "$FLAMEGRAPH_DIR"
    else
        cor_verde "  ✓ FlameGraph já presente em $FLAMEGRAPH_DIR"
    fi

    # hyperfine
    if ! command -v hyperfine &>/dev/null; then
        cor_amarela "  Instalando hyperfine..."
        sudo apt install -y hyperfine || cor_vermelha "  ⚠ Falha ao instalar hyperfine"
    else
        cor_verde "  ✓ hyperfine já instalado"
    fi

    # perf
    if ! command -v perf &>/dev/null; then
        cor_amarela "  Instalando perf..."
        sudo apt install -y linux-tools-common linux-tools-generic linux-tools-$(uname -r)
    else
        cor_verde "  ✓ perf já instalado"
    fi

    # py-spy
    if ! command -v py-spy &>/dev/null; then
        cor_amarela "  Instalando py-spy (para Python)..."
        pip install py-spy --break-system-packages 2>/dev/null \
            || pip install --user py-spy
    else
        cor_verde "  ✓ py-spy já instalado"
    fi

    cor_verde "✓ Setup concluído. Adicione ao seu .bashrc:"
    echo "    export PATH=\"$FLAMEGRAPH_DIR:\$PATH\""
}

# ===== BENCHMARK ESTATÍSTICO =====

cmd_bench() {
    local cmd="$1"
    [[ -z "$cmd" ]] && { cor_vermelha "Uso: $0 bench '<comando>'"; exit 1; }

    cor_amarela "▶ Benchmark estatístico de: $cmd"
    hyperfine --warmup 3 --runs 10 \
        --export-markdown "$SAIDA_DIR/bench.md" \
        --export-json "$SAIDA_DIR/bench.json" \
        "$cmd"

    cor_verde "✓ Resultados em: $SAIDA_DIR/bench.md"
}

# ===== FLAME GRAPH =====

cmd_flame() {
    local cmd="$1"
    [[ -z "$cmd" ]] && { cor_vermelha "Uso: $0 flame '<comando>'"; exit 1; }

    [[ ! -d "$FLAMEGRAPH_DIR" ]] && {
        cor_vermelha "FlameGraph não encontrado. Rode: $0 setup"
        exit 1
    }

    cor_amarela "▶ Coletando amostras com perf record (99 Hz)..."
    sudo perf record -F 99 -g -o "$SAIDA_DIR/perf.data" -- $cmd

    cor_amarela "▶ Convertendo para flame graph..."
    sudo perf script -i "$SAIDA_DIR/perf.data" \
        | "$FLAMEGRAPH_DIR/stackcollapse-perf.pl" \
        | "$FLAMEGRAPH_DIR/flamegraph.pl" \
        > "$SAIDA_DIR/flame.svg"

    cor_verde "✓ Flame graph: $SAIDA_DIR/flame.svg"
    cor_amarela "  Abra no navegador para interagir (zoom, busca por função)."
}

# ===== PERF STAT =====

cmd_stat() {
    local cmd="$1"
    [[ -z "$cmd" ]] && { cor_vermelha "Uso: $0 stat '<comando>'"; exit 1; }

    cor_amarela "▶ Coletando estatísticas arquiteturais..."
    sudo perf stat -e cycles,instructions,cache-references,cache-misses,\
branch-misses,page-faults,context-switches \
        $cmd 2>&1 | tee "$SAIDA_DIR/perf-stat.txt"

    echo ""
    cor_verde "✓ Estatísticas em: $SAIDA_DIR/perf-stat.txt"
    cor_amarela "  COMO LER:"
    echo "    • IPC alto (>1.5)         → pipeline saturado, código eficiente"
    echo "    • IPC baixo (<1.0)        → CPU esperando dados (cache miss?)"
    echo "    • cache-misses/refs >5%   → algoritmo cache-hostil"
    echo "    • branch-misses alto      → muitos if imprevisíveis"
}

# ===== FLUXO COMPLETO =====

cmd_full() {
    local cmd="$1"
    [[ -z "$cmd" ]] && { cor_vermelha "Uso: $0 full '<comando>'"; exit 1; }

    echo "=========================================="
    echo "PROFILING COMPLETO: $cmd"
    echo "=========================================="

    cmd_bench "$cmd"
    echo ""
    cmd_stat "$cmd"
    echo ""
    cmd_flame "$cmd"

    echo ""
    cor_verde "=========================================="
    cor_verde "✓ TUDO PRONTO. Resultados em: $SAIDA_DIR/"
    cor_verde "=========================================="
    ls -la "$SAIDA_DIR"
}

# ===== PYTHON ESPECÍFICO =====

cmd_python() {
    local cmd="$1"
    [[ -z "$cmd" ]] && { cor_vermelha "Uso: $0 python 'python3 script.py'"; exit 1; }

    cor_amarela "▶ Profiling Python com py-spy..."
    py-spy record -o "$SAIDA_DIR/flame-python.svg" -- $cmd

    cor_verde "✓ Flame graph Python: $SAIDA_DIR/flame-python.svg"
}

# ===== JAVA ESPECÍFICO =====

cmd_java() {
    local pid="$1"
    [[ -z "$pid" ]] && { cor_vermelha "Uso: $0 java <PID_JAVA>"; exit 1; }

    if [[ ! -d "$HOME/async-profiler" ]]; then
        cor_vermelha "async-profiler não encontrado em ~/async-profiler"
        cor_amarela "Baixe em: https://github.com/async-profiler/async-profiler/releases"
        exit 1
    fi

    cor_amarela "▶ Profiling Java por 30s..."
    "$HOME/async-profiler/profiler.sh" -d 30 -f "$SAIDA_DIR/flame-java.html" "$pid"
    cor_verde "✓ Flame graph Java: $SAIDA_DIR/flame-java.html"
}

# ===== DISPATCHER =====

case "${1:-}" in
    setup)  cmd_setup ;;
    bench)  shift; cmd_bench "$*" ;;
    flame)  shift; cmd_flame "$*" ;;
    stat)   shift; cmd_stat "$*" ;;
    full)   shift; cmd_full "$*" ;;
    python) shift; cmd_python "$*" ;;
    java)   cmd_java "$2" ;;
    *)
        echo "Uso: $0 {setup|bench|flame|stat|full|python|java} [comando]"
        echo ""
        echo "  setup                  Instala FlameGraph, hyperfine, perf, py-spy"
        echo "  bench  '<comando>'     Benchmark estatístico (10 runs + warmup)"
        echo "  flame  '<comando>'     Gera flame graph SVG"
        echo "  stat   '<comando>'     Estatísticas: IPC, cache-misses, etc."
        echo "  full   '<comando>'     Roda os três acima em sequência"
        echo "  python '<cmd>'         Flame graph para Python (via py-spy)"
        echo "  java   <PID>           Flame graph para Java (via async-profiler)"
        echo ""
        echo "Exemplos:"
        echo "  $0 setup"
        echo "  $0 full './meu_programa'"
        echo "  $0 python 'python3 meu_script.py'"
        exit 1 ;;
esac
