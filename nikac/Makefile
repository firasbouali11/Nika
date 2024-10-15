# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iheaders

# Directories
SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = build

# Source files
CORE_SRC = $(SRC_DIR)/core/lexer.c $(SRC_DIR)/core/parser.c $(SRC_DIR)/core/visitor.c
IO_SRC = $(SRC_DIR)/io/file_reader.c
UTILS_SRC = $(SRC_DIR)/utils/string_utils.c $(SRC_DIR)/utils/exception_utils.c
LIB_SRC = $(LIB_DIR)/linkedlist.c $(LIB_DIR)/list.c $(LIB_DIR)/map.c $(LIB_DIR)/queue.c $(LIB_DIR)/set.c $(LIB_DIR)/stack.c $(LIB_DIR)/heap.c
MAIN_SRC = $(SRC_DIR)/main.c

# Object files (in build directory)
CORE_OBJ = $(patsubst $(SRC_DIR)/core/%.c,$(BUILD_DIR)/%.o,$(CORE_SRC))
IO_OBJ = $(patsubst $(SRC_DIR)/io/%.c,$(BUILD_DIR)/%.o,$(IO_SRC))
UTILS_OBJ = $(patsubst $(SRC_DIR)/utils/%.c,$(BUILD_DIR)/%.o,$(UTILS_SRC))
LIB_OBJ = $(patsubst $(LIB_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRC))
MAIN_OBJ = $(BUILD_DIR)/main.o

# Output binary
TARGET = $(BUILD_DIR)/main

# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Linking
$(TARGET): $(CORE_OBJ) $(IO_OBJ) $(UTILS_OBJ) $(LIB_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

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

# Clean up
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
