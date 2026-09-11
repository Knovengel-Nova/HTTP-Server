CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -D_POSIX_C_SOURCE=200809L -Iinclude
LDFLAGS = -pthread

TARGET = server

SRC = src/main.c \
      src/server.c \
      src/client.c \
      src/connection.c\
	  src/request.c\
	  src/strings.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)