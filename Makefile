.SUFFIXES:

PREFIX ?= /usr/local
PREFIX := $(DESTDIR)$(PREFIX)

VERSION = $(shell git describe --tags --abbrev=0)
MAJOR_VERSION_NO = $(shell echo $(VERSION) | head -c 1)

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fpic -std=gnu99 -D_GNU_SOURCE
LDFLAGS = -lstatsdc

BUILD_DIR = build

all: $(BUILD_DIR) $(BUILD_DIR)/libstatsdc.so $(BUILD_DIR)/example

$(BUILD_DIR)/statsdc.o: src/statsdc.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -O2 -c -o $@ $<

$(BUILD_DIR)/libstatsdc.so: $(BUILD_DIR)/statsdc.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $<

$(BUILD_DIR)/example: src/example.c $(BUILD_DIR)/libstatsdc.so | $(BUILD_DIR)
	$(CC) $(CFLAGS) -Isrc -L$(BUILD_DIR) -o $@ $< $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

install: $(BUILD_DIR)/libstatsdc.so
	mkdir -p "$(PREFIX)/include"
	cp src/statsdc.h "$(PREFIX)/include/"

	mkdir -p "$(PREFIX)/lib"
	install "$(BUILD_DIR)/libstatsdc.so" "$(PREFIX)/lib/libstatsdc.so.$(VERSION)"

	ln -s "$(PREFIX)/lib/libstatsdc.so.$(VERSION)" "$(PREFIX)/lib/libstatsdc.so.$(MAJOR_VERSION_NO)"
	ln -s "$(PREFIX)/lib/libstatsdc.so.$(VERSION)" "$(PREFIX)/lib/libstatsdc.so"

uninstall:
	$(RM) "$(PREFIX)/lib/libstatsdc.so"
	$(RM) "$(PREFIX)/lib/libstatsdc.so.$(MAJOR_VERSION_NO)"
	$(RM) "$(PREFIX)/lib/libstatsdc.so.$(VERSION)"
	$(RM) "$(PREFIX)/include/libstatsdc.h"

.PHONY: all clean install uninstall
