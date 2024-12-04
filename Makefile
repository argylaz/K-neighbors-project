# Compiler
CC = g++

# Flags
CFLAGS = -Wall -g -O3


# Directories
TEST_DIR = tests
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build


# Test 
# Source Files
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
# Object Files
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRC)) 
# Executable
TEST_EXEC = $(patsubst $(TEST_DIR)/%.cpp, $(BIN_DIR)/%, $(TEST_SRC)) 

# Default target
.PHONY: all create test clean

all: create $(TEST_EXEC)

# Create Directories
create:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)

# Rule to compile test source files into object files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@


# Rule to build each test executable
$(BIN_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $<

run:
	$(CC) $(CFLAGS) -o bin/main main.cpp
	time ./bin/main  -f small -k 10 -r 10 -l 10 -a 1.2


# Run the test executable
test: $(TEST_EXEC)
	@for test_exec in $(TEST_EXEC); do \
		./$$test_exec; \
	done

valgrind: 
	make
	valgrind make test 

clean:
	rm -rf $(BUILD_DIR)/*.o 
	rm -rf $(BIN_DIR)/*


