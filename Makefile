# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Executable name
TARGET = seashell.out

# Source files
SRCS = $(shell find . -type f -name '*.c')

# Object files
OBJS = $(SRCS:.c=.o)

# Compile and link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

# Clean up
clean:
	rm -f $(TARGET) $(OBJS)
