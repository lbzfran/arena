
BIN := tester

SRC := ./src
BUILD := ./build
TEST := ./test

INC := -I./include
LIB := -L.
CFLAGS := -Wall -Wextra -Wpedantic
LDFLAGS := $(INC) $(LIB)
FLAGS := $(CFLAGS) $(LDFLAGS)

run: compile
	./build/$(BIN)-linear
	./build/$(BIN)-dynamic
	./build/$(BIN)-dynamic2

compile:
	cc -o "$(BUILD)/$(BIN)-linear" ./platform_linux.c $(TEST)/linear_alloc.c $(FLAGS)
	cc -o "$(BUILD)/$(BIN)-dynamic" ./platform_linux.c $(TEST)/dynamic_alloc.c $(FLAGS)
	cc -o "$(BUILD)/$(BIN)-dynamic2" ./platform_linux.c $(TEST)/dynamic_alloc2.c $(FLAGS)

clean:
	rm -f $(BUILD)/*
