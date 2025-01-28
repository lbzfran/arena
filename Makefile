
BIN := tester

SRC := ./src
BUILD := ./build
TEST := ./tests

INC := -I./include
LIB := -L.
CFLAGS := -Wall -Wextra -Wpedantic -ggdb
LDFLAGS := $(INC) $(LIB)
FLAGS := $(CFLAGS) $(LDFLAGS)

run: compile
	./build/$(BIN)-linear
	./build/$(BIN)-dynamic
	# ./build/$(BIN)-dynamic2

compile:
	cc -o "$(BUILD)/$(BIN)-linear" $(TEST)/linear_alloc.c $(FLAGS)
	cc -o "$(BUILD)/$(BIN)-dynamic" $(SRC)/arena_memory_linux.c $(TEST)/dynamic_alloc.c $(FLAGS)
	# cc -o "$(BUILD)/$(BIN)-dynamic2" $(SRC)/arena_memory_linux.c $(TEST)/dynamic_alloc2.c $(FLAGS)

clean:
	rm -f $(BUILD)/*
