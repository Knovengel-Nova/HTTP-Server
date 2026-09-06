CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -Iinclude

TARGET = server

SRC = src/main.c \
      src/server.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)