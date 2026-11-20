#!/bin/bash
set -e

export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:$PATH"
cd "$(dirname "$0")"

echo "=== Compilazione Avventura In Montagna (Mountain Quest) ==="
cmake -B build
cmake --build build

echo "Avvio AvventuraInMontagna..."
./build/AvventuraInMontagna "$@"
