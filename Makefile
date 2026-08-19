SRC_DIR := src
BUILD_DIR := build/
INTF_DIR := src/intf
IMPL_DIR := src/impl
TARGET := tiger

CC := gcc
CFLAGS := -Wall -Wextra -O2 -I$(INTF_DIR) -g

SRCS := $(shell find $(IMPL_DIR) -name '*.c')
OBJS := $(patsubst $(IMPL_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(IMPL_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

