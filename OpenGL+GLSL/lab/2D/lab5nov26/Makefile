.PHONY: all build run-modeler run-game clean

all: build

build:
	@echo "=== Compilazione Parte I: Modellatore Hermite 2D ==="
	@cd Modellatore_Hermite_2D && ./run.sh --build-only 2>/dev/null || (export PATH="/opt/homebrew/bin:/usr/local/bin:$$PATH" && cmake -B build && cmake --build build)
	@echo "\n=== Compilazione Parte II: Cannone Hermite 2D ==="
	@cd Parte_II_Cannone_Hermite && export PATH="/opt/homebrew/bin:/usr/local/bin:$$PATH" && cmake -B build && cmake --build build

run-modeler:
	@cd Modellatore_Hermite_2D && ./run.sh

run-game:
	@cd Parte_II_Cannone_Hermite && ./run.sh

clean:
	rm -rf Modellatore_Hermite_2D/build Modellatore_Hermite_2D/Modellatore_Hermite_2D
	rm -rf Parte_II_Cannone_Hermite/build Parte_II_Cannone_Hermite/Cannone_Hermite_2D
