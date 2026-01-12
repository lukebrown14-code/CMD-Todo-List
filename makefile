# Makefile for simple todo list project

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS =

# Name of the final executable
TARGET = todo

# Source files
SOURCES = src/array.c src/main.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
%.o: %.c array.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe

# Phony targets (not real files)
.PHONY: all clean

# Run the program and pass ALL extra make arguments directly
run: $(TARGET)
	./$(TARGET) $(filter-out $@,$(MAKECMDGOALS))

# Prevent make from treating arguments as targets
%:
	@:

# Debug build
debug: CFLAGS += -g -O0
debug: clean all
