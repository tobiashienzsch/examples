.PHONY: all clean

BINARY_NAME=cxx-static-vector
OBJ := main.o

CXX_STANDARD=-std=c++17
CXX_WARNINGS=-Wall -Wextra -Wpedantic
CXX_OPTIMIZATIONS=-O0 --coverage

COMPILER_OPTIONS=$(CXX_STANDARD) $(CXX_OPTIMIZATIONS) $(CXX_WARNINGS)

all: $(BINARY_NAME)

$(BINARY_NAME): $(OBJ)
	$(CXX) -o $@ $^ $(COMPILER_OPTIONS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(COMPILER_OPTIONS)

clean:
	rm -f $(OBJ)
	rm -f $(BINARY_NAME)
	rm -rf ./lcov *.info *.gcda *.gcno
