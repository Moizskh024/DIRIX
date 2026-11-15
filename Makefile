CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
SRC = src/main.c src/tree.c src/storage.c
TARGET = dirix

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
