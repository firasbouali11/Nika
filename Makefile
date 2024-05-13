# Compiler
CC = gcc

# Directories
SRC_DIR = src
LIB_DIR = lib
IO_DIR = io

# Source files
MAIN_SRC = main.c
LIB_SRCS = LinkedList.c Queue.c Stack.c Map.c List.c Set.c
SRC_SRCS = Lexer.c Parser.c Visitor.c
IO_SRCS = FileReader.c

# Object files
MAIN_OBJ = $(MAIN_SRC:.c=.o)
LIB_OBJS = $(LIB_SRCS:.c=.o)
SRC_OBJS = $(SRC_SRCS:.c=.o)
IO_OBJS = $(IO_SRCS:.c=.o)

# Executable
OUTPUT = main

# Compilation flags
CFLAGS = -Wall -Wextra -pedantic -g
# CFLAGS = -O2 -s -DNDEBUG -fomit-frame-pointer -march=native

# Targets
all: $(OUTPUT)

$(OUTPUT): $(MAIN_OBJ) $(LIB_OBJS) $(SRC_OBJS) $(IO_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(MAIN_OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OBJS): %.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_OBJS): %.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(IO_OBJS): %.o: $(IO_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUTPUT) $(MAIN_OBJ) $(LIB_OBJS) $(SRC_OBJS) $(IO_OBJS)
