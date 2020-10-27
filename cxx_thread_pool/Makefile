WARNINGS ?= -Wall -Wextra -Wpedantic
OPTIMIZER ?= -O2 -flto -fno-omit-frame-pointer
LINKER ?= -lpthread
CXX_FLAGS ?= -std=c++17 $(WARNINGS) $(OPTIMIZER) $(LINKER)

ASAN ?= -fsanitize=address
UBSAN ?= -fsanitize=undefined
TSAN ?= -fsanitize=thread

SOURCE += main.cpp
SOURCE += pool.cpp

.PHONY: asan
asan:
	$(CXX) $(ASAN) $(CXX_FLAGS) $(SOURCE)

.PHONY: ubsan
ubsan:
	$(CXX) $(UBSAN) $(CXX_FLAGS) $(SOURCE)

.PHONY: tsan
tsan:
	$(CXX) $(TSAN) $(CXX_FLAGS) $(SOURCE)