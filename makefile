CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11

TARGET = todo
SOURCES = src/array.c src/main.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
%.o: %.c src/array.h
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
