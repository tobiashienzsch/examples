.PHONY: default
default: config build

.PHONY: config
config:
	cmake -S. -GNinja -Bbuild

.PHONY: build
build:
	cmake --build build

.PHONY: clean
clean:
	rm -rf build
