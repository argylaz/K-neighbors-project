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

all: clean create $(TEST_EXEC)

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

run: clean
	$(CC) $(CFLAGS) -o bin/main main.cpp
# ./bin/main  -f dummy -k 100 -L 120 -R 30 -a 1.2 -v filtered -Rst stitched 
# time ./bin/main  -f dummy -k 100 -L 120 -R 30 -a 1.2 -v filtered > run.txt
	time ./bin/main  -f small -k 100 -L 120 -R 30 -a 1.2 -v simple

# Run the test executable
test: clean $(TEST_EXEC)
	@for test_exec in $(TEST_EXEC); do \
		./$$test_exec; \
	done


# Create groundtruth.bin file
groundtruth:
	$(CC) $(CFLAGS) -o bin/groundtruth groundtruth.cpp
	./bin/groundtruth


g_test:
	$(CC) $(CFLAGS) -o bin/groundtruth_reading_test groundtruth_reading_test.cpp
	./bin/groundtruth_reading_test


# Run tests with valgrind
valgrind: 
	valgrind make test 

# Cleaning everything in bin and build folders except *_graph.bin files,
# which contain saved Graphs made with Filtered, Stiched or Simple Vamana algorithms  
clean:
	rm -rf $(BUILD_DIR)/*.o
	find $(BIN_DIR) -type f ! -name '*_graph.bin' ! -name 'groundtruth.bin' -delete 

# Erase everything in bin and build folders
erase:
	rm -rf $(BUILD_DIR)/*.o
	rm -rf $(BIN_DIR)/*
	



