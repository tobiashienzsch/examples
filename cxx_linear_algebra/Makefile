BIN_DIR ?= bin

.PHONY: all
all: geometry

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: geometry
geometry: $(BIN_DIR)
	$(CXX) -std=c++2a -O3 -DNDEBUG -mavx -Wall -Wextra -Wpedantic -o $(BIN_DIR)/linear_algebra src/main.cpp

.PHONY: coverage
coverage: tests
	$(CXX) -std=c++2a -O0 -g3 -coverage -Wall -Wextra -Wpedantic -o $(BIN_DIR)/tests-coverage src/test.cpp
	./$(BIN_DIR)/tests-coverage

.PHONY: coverage-html
coverage-html: coverage
	mkdir -p $(BIN_DIR)/coverage
	gcovr --html --html-details --exclude-unreachable-branches -o bin/coverage/coverage.html -r .

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
	rm -rf *.gcno *.gcda