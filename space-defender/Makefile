CC      ?= gcc
CFLAGS  ?= -std=c11 -D_DEFAULT_SOURCE -Wall -Wextra -pedantic -O2
CFLAGS  += -Iinclude
LDLIBS  += -lncurses

TARGET  := space_defender
SRC     := $(wildcard src/*.c)
OBJ     := $(SRC:src/%.c=build/%.o)
TESTS   := build/test_highscore build/test_difficulty

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

build/%.o: src/%.c $(wildcard include/*.h) | build
	$(CC) $(CFLAGS) -c $< -o $@

build/test_%: tests/test_%.c src/%.c $(wildcard include/*.h) | build
	$(CC) $(CFLAGS) tests/test_$*.c src/$*.c -o $@

build:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)

test: $(TESTS)
	cd build && ./test_highscore && ./test_difficulty

clean:
	rm -rf build $(TARGET) highscores.txt

.PHONY: all run test clean
