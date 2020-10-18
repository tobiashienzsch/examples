CONFIG ?= Release
BUILD_DIR ?= build
CM_GENERATOR ?= Ninja

.PHONY: all
all: config build

.PHONY: config
config:
	cmake -B$(BUILD_DIR) -G$(CM_GENERATOR) -S. -DCMAKE_BUILD_TYPE=$(CONFIG)

.PHONY: build
build:
	cmake --build $(BUILD_DIR) --config $(CONFIG)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
