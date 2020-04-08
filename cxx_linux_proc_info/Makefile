
PROJECT=proc-info
PROJECT_OBJ := main.o

CXX_STANDARD=-std=c++17
CXX_WARNINGS=-Wall -Wextra -Wpedantic
CXX_OPTIMIZATIONS=-march=native -Os

COMPILER_OPTIONS=$(CXX_STANDARD) $(CXX_OPTIMIZATIONS) $(CXX_WARNINGS)
LINKER_OPTIONS=-pthread

.PHONY: all
all: $(PROJECT) 

$(PROJECT): $(PROJECT_OBJ)
	$(CXX) -o $@ $^ $(COMPILER_OPTIONS) $(LINKER_OPTIONS)


%.o: %.cpp
	$(CXX) -c $< -o $@ $(COMPILER_OPTIONS)

.PHONY: clean
clean:
	rm -f $(PROJECT_OBJ)
	rm -f $(PROJECT)
