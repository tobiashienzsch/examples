CONFIG ?= Release

.PHONY: all
all: config build install

.PHONY: config
config:
	cmake -H. -GNinja -Bbuild -DCMAKE_BUILD_TYPE=$(CONFIG) -DCMAKE_INSTALL_PREFIX=./install

.PHONY: build
build:
	cmake --build build

.PHONY: install
install:
	cmake --build build --target install

.PHONY: package
package:
	cmake --build build --target package_source
	
.PHONY: clean
clean:
	rm -rf build install