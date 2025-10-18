# Makefile for Virtual File System

CC = gcc
CFLAGS = -Wall -Wextra -fPIC
TARGET_CLI = vfs_cli
TARGET_SO = libvfs.so

all: $(TARGET_SO) $(TARGET_CLI)

$(TARGET_SO): functions.c helper.c headers.h
	$(CC) $(CFLAGS) -shared -o $(TARGET_SO) functions.c helper.c

$(TARGET_CLI): main.c functions.c helper.c headers.h
	$(CC) $(CFLAGS) -o $(TARGET_CLI) main.c functions.c helper.c

clean:
	rm -f $(TARGET_CLI) $(TARGET_SO) *.o

.PHONY: all clean