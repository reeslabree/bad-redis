CC = clang
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
INCLUDE = -Iinclude
BUILD_DIR = build

# Source files
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))
BIN = $(BUILD_DIR)/badredis

# Test files
TESTS = $(wildcard tests/*.c)
TEST_OBJ = $(patsubst tests/%.c,$(BUILD_DIR)/%.o,$(TESTS))

# Build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Compile test files
$(BUILD_DIR)/%.o: tests/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Application binary
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

# Clean
clean:
	rm -rf $(BUILD_DIR)/*

# Run application
run: $(BIN)
	./$(BIN)

# Test names (without test_ prefix and .c extension)
TEST_NAMES = kv store

# Run all tests
.PHONY: test
test: $(addprefix test-,$(TEST_NAMES))
	@echo "All tests completed"

# Run a single test, e.g., make test-kv
.PHONY: test-kv test-store
test-kv: $(filter-out $(BUILD_DIR)/main.o,$(OBJ)) $(BUILD_DIR)/test_kv.o
	$(CC) $(filter-out $(BUILD_DIR)/main.o,$(OBJ)) $(BUILD_DIR)/test_kv.o -o $(BUILD_DIR)/test_kv -lpthread
	./$(BUILD_DIR)/test_kv

test-store: $(filter-out $(BUILD_DIR)/main.o,$(OBJ)) $(BUILD_DIR)/test_store.o
	$(CC) $(filter-out $(BUILD_DIR)/main.o,$(OBJ)) $(BUILD_DIR)/test_store.o -o $(BUILD_DIR)/test_store -lpthread
	./$(BUILD_DIR)/test_store
