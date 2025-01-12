# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iheaders

# Directories
SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = build
TEST_DIR = test

# Source files
CORE_SRC = $(shell find $(SRC_DIR)/core -maxdepth 1 -name *.c -type f)
IO_SRC = $(SRC_DIR)/io/file_reader.c
UTILS_SRC = $(shell find $(SRC_DIR)/utils -maxdepth 1 -name *.c -type f)
LIB_SRC = $(shell find $(LIB_DIR) -maxdepth 1 -name *.c -type f)
MAIN_SRC = $(SRC_DIR)/main.c
UNITY_SRC = $(TEST_DIR)/unity/unity.c
TEST_SRC = $(shell find $(TEST_DIR) -maxdepth 1 -name *.c -type f)

# Object files (in build directory)
CORE_OBJ = $(patsubst $(SRC_DIR)/core/%.c,$(BUILD_DIR)/%.o,$(CORE_SRC))
IO_OBJ = $(patsubst $(SRC_DIR)/io/%.c,$(BUILD_DIR)/%.o,$(IO_SRC))
UTILS_OBJ = $(patsubst $(SRC_DIR)/utils/%.c,$(BUILD_DIR)/%.o,$(UTILS_SRC))
LIB_OBJ = $(patsubst $(LIB_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRC))
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRC))
UNITY_OBJ = $(BUILD_DIR)/unity.o
MAIN_OBJ = $(BUILD_DIR)/main.o

# Output binary
TARGET = $(BUILD_DIR)/main

# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	echo $(TEST_OBJ)
	mkdir -p $(BUILD_DIR)

# Linking
$(TARGET): $(CORE_OBJ) $(IO_OBJ) $(UTILS_OBJ) $(LIB_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

test: $(TEST_OBJ) $(LIB_OBJ) $(UTILS_OBJ) $(UNITY_OBJ) $(IO_OBJ) $(CORE_OBJ)
	$(CC) -Iheaders -o $(BUILD_DIR)/test $^ && ./$(BUILD_DIR)/test

# Compiling source files into build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/core/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/io/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/utils/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(MAIN_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/unity.o: $(UNITY_SRC) | $(BUILD_DIR)
	$(CC) -Iheaders -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) -Iheaders -c $< -o $@
	
# Clean up
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
