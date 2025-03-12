# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g

# Source files
SRC = color.c display.c exec_basic.c execute_cmd.c hop.c log.c main.c proclore.c prompt.c reveal.c seek.c

# Object files
OBJ = $(SRC:.c=.o)

# Output executable
TARGET = shell

# Default target
all: $(TARGET)

# Linking the object files to create the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile each .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up the build
clean:
	rm -f $(OBJ) $(TARGET)

# Running the program
run:
	./$(TARGET)

.PHONY: all clean run
