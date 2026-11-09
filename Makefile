# ══════════════════════════════════════════════════════════════════
#  SnaCe - CLI Snake Game in C
#  Makefile
# ══════════════════════════════════════════════════════════════════

CC       = gcc
CFLAGS   = -std=c11 -Wall -Wextra -Wpedantic -Isrc
LDFLAGS  =
SRC_DIR  = src
BUILD_DIR = build
TARGET   = snace

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/game.c \
       $(SRC_DIR)/snake.c \
       $(SRC_DIR)/food.c \
       $(SRC_DIR)/input.c \
       $(SRC_DIR)/ui.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# ── Default target ──────────────────────────────────────────────
all: $(BUILD_DIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# ── Run the game ────────────────────────────────────────────────
run: all
	./$(TARGET)

# ── Debug build ─────────────────────────────────────────────────
debug: CFLAGS += -g -DDEBUG
debug: clean all
	@echo "Debug build complete (with symbols)."

# ── Clean ───────────────────────────────────────────────────────
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TARGET).exe
	@echo "Clean complete."

# ── Phony targets ───────────────────────────────────────────────
.PHONY: all run debug clean

