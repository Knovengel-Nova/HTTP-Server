CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -Iinclude
LDFLAGS = -pthread

TARGET = server

SRC = src/main.c \
      src/server.c \
      src/client.c \
      src/connection.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)