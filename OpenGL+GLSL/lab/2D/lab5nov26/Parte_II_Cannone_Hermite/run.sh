#!/bin/bash
set -e

# Assicura che i tool di Homebrew (cmake, pkg-config, ecc.) siano nel PATH
export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:$PATH"

# Naviga nella cartella del progetto (dove risiede questo script)
cd "$(dirname "$0")"

# Configura e compila con CMake
cmake -B build
cmake --build build

# Lancia l'applicazione (a meno che non sia specificato --build-only)
if [ "$1" != "--build-only" ]; then
    ./Cannone_Hermite_2D "$@"
fi
