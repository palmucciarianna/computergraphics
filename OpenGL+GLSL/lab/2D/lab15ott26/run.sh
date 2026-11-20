#!/bin/bash
set -e

# Assicura che i tool di Homebrew (cmake, pkg-config, ecc.) siano nel PATH
export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:$PATH"

# Naviga nella cartella del progetto (dove risiede questo script)
cd "$(dirname "$0")"

# Configura e compila con CMake
cmake -B build
cmake --build build

# Lancia l'applicazione
./GrigliaAnimata2D "$@"
