# Compiler
CC = g++

# Flags
# CFLAGS = -Wall -g -Iinclude


# Directories
TEST_DIR = tests
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build


# Test
# Source Files
TEST_SRC = $(TESTS_DIR)/test_graph.cpp 
# Object Files
TEST_OBJ = $(BUILD_DIR)/test_graph.o 
# Executable
TEST = $(BIN_DIR)/test_graph

# # Server 
# # Source Files
# SERVER_SRC = $(SRC_DIR)/jobExecutorServer.c $(SRC_DIR)/safe_func.c $(SRC_DIR)/job.c $(SRC_DIR)/auxiliary_func.c
# # Object Files
# SERVER_OBJ = $(BUILD_DIR)/jobExecutorServer.o $(BUILD_DIR)/safe_func.o $(BUILD_DIR)/job.o $(BUILD_DIR)/auxiliary_func.o
# # Executable
# SERVER = $(BIN_DIR)/jobExecutorServer

# Default
# all: $(COMMANDER) $(SERVER)

create:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)

test: $(TEST)

# server: $(SERVER)

# progDelay: $(PROGDELAY)

# Rule for Commander
$(TEST): $(TEST_OBJ)
	$(CC) -o $(TEST) $(TEST_OBJ)


# # Rule for Server
# $(SERVER): $(SERVER_OBJ)
# 	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_OBJ) -lpthread

# # Rule for ProgDelay
# $(PROGDELAY): $(PROGDELAY_OBJ)
# 	$(CC) $(CFLAGS) -o $(PROGDELAY) $(PROGDELAY_OBJ)


# Compile source to object files
$(BUILD_DIR)/test_graph.o: $(TEST_DIR)/test_graph.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# $(BUILD_DIR)/safe_func.o: $(SRC_DIR)/safe_func.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(BUILD_DIR)/jobExecutorServer.o: $(SRC_DIR)/jobExecutorServer.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(BUILD_DIR)/job.o: $(SRC_DIR)/job.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(BUILD_DIR)/auxiliary_func.o: $(SRC_DIR)/auxiliary_func.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(BUILD_DIR)/progDelay.o: $(SRC_DIR)/progDelay.c
# 	$(CC) $(CFLAGS) -c $< -o $@


# clean:
# 	rm -f $(SERVER_OBJ) $(SERVER) $(COMMANDER_OBJ) $(COMMANDER) 

# clean_progDelay:
# 	rm -f $(PROGDELAY_OBJ) $(PROGDELAY)



# all:
# 	gcc -Wall -g -Iinclude src/safe_func.c src/jobCommander.c -o bin/jobCommander 
# 	gcc -Wall -g -Iinclude src/job.c src/safe_func.c src/jobExecutorServer.c -lpthread -o bin/jobExecutorServer 


# commander:
# 	gcc -Wall -g -Iinclude src/safe_func.c src/jobCommander.c -o bin/jobCommander

# server:
# 	gcc -Wall -g -Iinclude src/job.c src/safe_func.c src/jobExecutorServer.c -lpthread -o bin/jobExecutorServer



test_run:
	./$(TEST)
