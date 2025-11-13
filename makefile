# Makefile for Virtual File System with Memory Management

CC = gcc
CFLAGS = -Wall -Wextra -fPIC
TARGET_CLI = vfs_cli
TARGET_SO = libvfs.so

all: $(TARGET_SO) $(TARGET_CLI)

$(TARGET_SO): functions.c helper.c memory_manager.c headers.h
	$(CC) $(CFLAGS) -shared -o $(TARGET_SO) functions.c helper.c memory_manager.c

$(TARGET_CLI): main.c functions.c helper.c memory_manager.c headers.h
	$(CC) $(CFLAGS) -o $(TARGET_CLI) main.c functions.c helper.c memory_manager.c

clean:
	rm -f $(TARGET_CLI) $(TARGET_SO) *.o vfs_backup.dat

install: $(TARGET_SO)
	cp $(TARGET_SO) /usr/local/lib/ || echo "Please run as root for installation"

.PHONY: all clean install
