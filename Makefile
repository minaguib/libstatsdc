.SUFFIXES:

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fpic -std=gnu99 -D_GNU_SOURCE
LDFLAGS = -lstatsdc

all: build build/libstatsdc.so build/example

build/statsdc.o: src/statsdc.c
	$(CC) $(CFLAGS) -O2 -c -o $@ $<

build/libstatsdc.so: build/statsdc.o
	$(CC) -shared -o $@ $<

build/example: src/example.c build/libstatsdc.so
	$(CC) $(CFLAGS) -Isrc -Lbuild -o $@ $< $(LDFLAGS)

build:
	mkdir -p $@

clean:
	rm -rf build

.PHONY: all clean
