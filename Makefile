.SUFFIXES:

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

.PHONY: all clean
