.PHONY: all build run clean

all: build

build:
	@export PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:$$PATH" && \
	cmake -B build && \
	cmake --build build

run: build
	./Cannone_Hermite_2D

clean:
	rm -rf build Cannone_Hermite_2D
