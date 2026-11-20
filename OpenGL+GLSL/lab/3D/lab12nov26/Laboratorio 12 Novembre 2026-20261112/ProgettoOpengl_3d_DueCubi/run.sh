#!/bin/bash
set -e

export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:$PATH"
cd "$(dirname "$0")"

echo "=== Compilazione Progetto OpenGL 3D Due Cubi ==="
cmake -B build
cmake --build build

echo "Avvio ProgettoOpengl_3d_DueCubi..."
./build/ProgettoOpengl_3d_DueCubi "$@"
