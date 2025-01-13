
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
	./build/$(BIN)

compile:
	cc -o $(BUILD)/$(BIN) $(TEST)/alloc_tester.c $(SRC)/alloc.c $(FLAGS)

clean:
	rm -f $(BUILD)/*
