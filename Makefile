CC = gcc
ASAN_FLAGS =
ifeq ($(ASAN),1)
    ASAN_FLAGS = -fsanitize=address -fsanitize-trap
else ifeq ($(filter asan,$(MAKECMDGOALS)),asan)
    ASAN_FLAGS = -fsanitize=address -fsanitize-trap
endif

CFLAGS = -Wall -Wextra -Wpedantic -Wcast-align -Wconversion -Wunused -Wshadow -O2 $(ASAN_FLAGS) -std=c11 -Iinclude
LDFLAGS = $(ASAN_FLAGS)

BIN = software_rasterizer

SRCDIR = src
INCDIR = include
OBJDIR = obj

C_FILES = $(wildcard *.c) $(wildcard $(SRCDIR)/*.c)

OBJ_FILES = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(C_FILES)))

all: $(BIN)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BIN): $(OBJ_FILES) | $(OBJDIR)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(BIN)

re: fclean all

asan:
	@$(MAKE) all ASAN=1

.PHONY: all clean fclean re asan
