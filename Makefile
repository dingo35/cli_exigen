# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Target binary name
TARGET = hex_to_bitstream

# Automatically find all C source files in the current directory
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

# Default target
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up object files and the executable
clean:
	rm -f $(TARGET) $(OBJS)

