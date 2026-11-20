.PHONY: all build run clean

all: build

build:
	@export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:$$PATH"; \
	cmake -B build && cmake --build build

run: build
	@./Modellatore_Hermite_2D

clean:
	@rm -rf build Modellatore_Hermite_2D
