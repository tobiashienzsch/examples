.PHONY: all
all: build run

.PHONY: build
build:
	$(CXX) -std=c++17 -O3 -Wall -Wextra -Wpedantic -ldispatch main.cpp 

.PHONY: run
run:
	/usr/bin/time ./a.out | sort | uniq -c