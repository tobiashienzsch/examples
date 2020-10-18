LLVM_DIR=/usr
LLVM_VERSION=10

.PHONY: build
build:
	@cmake -H. -Bbuild -GNinja -DLT_LLVM_INSTALL_DIR=$(LLVM_DIR) 
	@cmake --build build

.PHONY: run
run:
	$(LLVM_DIR)/bin/clang-$(LLVM_VERSION) -O3 -S -emit-llvm hello.cpp -o hello.ll
	$(LLVM_DIR)/bin/opt-$(LLVM_VERSION) -load-pass-plugin ./build/libHelloWorld.so -passes=hello-world -disable-output hello.ll