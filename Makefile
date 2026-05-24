CC = gcc
CFLAGS = -Wall -Wextra -g
INCLUDES = -Iinclude

TARGET = program.exe
TEST_TARGET = tests.exe

SRCS = src/backendGraph.c src/dsu.c src/front.c src/main.c src/mst_algorithms.c src/mst_compare.c
TEST_SRCS = tests/testsBackendGr.c src/backendGraph.c src/dsu.c src/front.c src/mst_algorithms.c src/mst_compare.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SRCS)

test: $(TEST_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TEST_TARGET) $(TEST_SRCS)
	./$(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	del $(TARGET) $(TEST_TARGET) *.o


