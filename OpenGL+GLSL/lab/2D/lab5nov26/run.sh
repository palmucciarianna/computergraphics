#!/bin/bash
set -e

export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:$PATH"

cd "$(dirname "$0")"

# Di default avvia il Modellatore Hermite 2D (Parte I)
cd Modellatore_Hermite_2D
./run.sh "$@"
