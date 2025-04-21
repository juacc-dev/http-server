MAKEFLAGS += --jobs=12

CC = gcc
CFLAGS = -Wall -Wextra -g3
LDFLAGS = -lescape -lluajit-5.1

TARGET = server

SRC_DIR = ./src
SRCS = $(shell find $(SRC_DIR) -name '*.c')
DEPS = $(COMMON_DIR)/error.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

clean:
	rm -rf $(TARGET)
