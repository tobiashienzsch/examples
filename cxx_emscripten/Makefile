BUILD_DIR ?= bin

.PHONY: all
all: build

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: build
build: $(BUILD_DIR)
	em++ -std=c++20 -Os -flto -Wall -Wextra -Wpedantic src/main.cpp -o $(BUILD_DIR)/main.js
	em++ -std=c++20 -Os -flto -Wall -Wextra -Wpedantic src/gui.cpp -o $(BUILD_DIR)/gui.html

.PHONY: run
run:
	node $(BUILD_DIR)/main.js

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

