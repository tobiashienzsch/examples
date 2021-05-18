BIN_DIR ?= bin

.PHONY: all
all: linear_alegra tests

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: linear_alegra
linear_alegra: $(BIN_DIR)
	$(CXX) -std=c++2a -O3 -mavx -Wall -Wextra -Wpedantic -o $(BIN_DIR)/linear_algebra main.cpp

.PHONY: tests
tests: $(BIN_DIR)
	$(CXX) -std=c++2a -O3 -mavx -Wall -Wextra -Wpedantic -o $(BIN_DIR)/tests test.cpp

.PHONY: coverage
coverage: tests
	$(CXX) -std=c++2a -O0 -coverage -Wall -Wextra -Wpedantic -o $(BIN_DIR)/tests-coverage test.cpp
	./$(BIN_DIR)/tests-coverage

.PHONY: coverage-html
coverage-html: coverage
	mkdir -p $(BIN_DIR)/coverage
	gcovr --html --html-details --exclude-unreachable-branches -o bin/coverage/coverage.html -r .

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
	rm -rf *.gcno *.gcda