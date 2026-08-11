CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC = src/main.c
TARGET = bin/devshell

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -rf bin/*
