.PHONY: all clean

BINARY_NAME=cxx-runtime-polymorphism
OBJ := main.o
OBJ += document.o

CXX_STANDARD=-std=c++2a
CXX_WARNINGS=-Wall -Wextra -Wpedantic
CXX_OPTIMIZATIONS=-march=native -Os

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
