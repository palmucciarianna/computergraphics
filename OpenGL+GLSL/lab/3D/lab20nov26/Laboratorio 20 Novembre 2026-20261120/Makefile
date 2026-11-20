.PHONY: all build run clean

all: build

build:
	@export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:$$PATH"; \
	cmake -B build && cmake --build build

run: build
	@./Progetto3D_Interattivo

clean:
	@rm -rf build Progetto3D_Interattivo
