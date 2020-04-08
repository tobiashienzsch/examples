.PHONY: all clean

BINARY_NAME=cxx-high-resolution-timer
OBJ := main.o

CXX_STANDARD=-std=c++2a
CXX_WARNINGS=-Wall -Wextra -Wpedantic
CXX_OPTIMIZATIONS=-march=native -O3

COMPILER_OPTIONS=$(CXX_STANDARD) $(CXX_OPTIMIZATIONS) $(CXX_WARNINGS)
LINKER_OPTIONS=-pthread

all: $(BINARY_NAME)

$(BINARY_NAME): $(OBJ)
	$(CXX) -o $@ $^ $(COMPILER_OPTIONS) $(LINKER_OPTIONS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(COMPILER_OPTIONS)

clean:
	rm -f $(OBJ)
	rm -f $(BINARY_NAME)
