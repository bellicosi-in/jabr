CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = -lm

SOURCES = matrix.c
TEST_SOURCES = test_matrix.c matrix.c
HEADERS = matrix.h

TARGET = test_matrix
OBJECTS = $(SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

help:
	@echo "Available targets:"
	@echo "  all      - Build the test executable"
	@echo "  test     - Build and run tests"
	@echo "  clean    - Remove built files"
	@echo "  valgrind - Run tests with memory leak detection"
	@echo "  help     - Show this help message"