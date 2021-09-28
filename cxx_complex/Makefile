.PHONY: all
all: clean build run

.PHONY: build
build:
	${CXX} -std=c++17 -Og -g -Wall -Wextra -Wpedantic -o test main.cpp

.PHONY: run
run:
	./test

.PHONY: clean
clean:
	rm -rf test

