#!/bin/bash
set -e

export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:$PATH"
cd "$(dirname "$0")"

echo "=== Compilazione Laboratorio 22 Ottobre 2026 (Cannone & Bersagli) ==="
cmake -B build
cmake --build build

TARGET="cannone_svolto"
if [ "$1" == "starter" ]; then
    TARGET="cannone"
fi

echo "Avvio $TARGET..."
./build/$TARGET
