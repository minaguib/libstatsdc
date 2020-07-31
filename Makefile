.SUFFIXES:

SHELL = /bin/sh

PREFIX ?= /usr/local
LIBDIR := $(DESTDIR)$(PREFIX)/lib
INCDIR := $(DESTDIR)$(PREFIX)/include

VERSION          := $(shell git describe --tags --abbrev=0)
MAJOR_VERSION_NO := $(shell echo $(VERSION) | head -c 1)

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
	mkdir -p $(INCDIR)
	cp src/statsdc.h $(INCDIR)

	mkdir -p $(LIBDIR)
	install "$(BUILD_DIR)/libstatsdc.so" $(LIBDIR)/libstatsdc.so.$(VERSION)
	ln -s "$(LIBDIR)/libstatsdc.so.$(VERSION)" "$(LIBDIR)/libstatsdc.so.$(MAJOR_VERSION_NO)"
	ln -s "$(LIBDIR)/libstatsdc.so.$(VERSION)" "$(LIBDIR)/libstatsdc.so"

uninstall:
	$(RM) "$(LIBDIR)/libstatsdc.so"
	$(RM) "$(LIBDIR)/libstatsdc.so.$(MAJOR_VERSION_NO)"
	$(RM) "$(LIBDIR)/libstatsdc.so.$(VERSION)"
	$(RM) "$(INCDIR)/libstatsdc.h"

.PHONY: all clean install uninstall
