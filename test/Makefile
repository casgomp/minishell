# === EXECUTABLE NAME ===
#NAME            := <div testing executables>

# === CONFIGURATION ===
CC              := cc
RM              := rm -f
MKDIR           := /usr/bin/mkdir -p
MAKE			:= make -L

# === DIRECTORIES ===
# Object file directories
OBJ_DIR			:= obj
# Library & Include directories
LIBFT_DIR       := ../libft
INC_DIR         := include
# Source directories for vpath
SRC_DIRS        := src ../src

# === LIBRARIES ===
LIBFT			:= $(LIBFT_DIR)/libft.a

# === FLAGS ===
CFLAGS          := -Wall -Wextra -Werror -O3 -MD
CFLAGS			+= -g3 -O0
CPPFLAGS        := -I$(INC_DIR) -I../$(INC_DIR) -I$(LIBFT_DIR)/$(INC_DIR)
LDFLAGS         :=  -L$(LIBFT_DIR)
LDLIBS          :=  -lft -lreadline

# === SOURCE FILES ===
# Use vpath to tell 'make' where to find source files
vpath %.c $(SRC_DIRS)

SRCFILES	:= 	test_split_quote_expand.c \
				ms_rmquotes_expand.c

# === OBJECT FILES ===
# Create separate object lists for each target
OBJFILES  := $(addprefix $(OBJ_DIR)/, $(SRCFILES:.c=.o))

# Dependency files for header changes
DEPS            := $(OBJFILES:.o=.d) $(OBJS_BONUS:.o=.d)

# --- COLORS ---
GREEN           := \033[0;32m
YELLOW          := \033[0;33m
BLUE_BOLD       := \033[1;34m
RESET           := \033[0m

# === MAIN RULES ===
## .DEFAULT_GOAL   := all
all: test_split_quote_expand

-include $(DEPS)

# --- TARGETS ---

test_split_quote_expand: test_split_quote_expand.o ms_rmquotes_expand.o $(LIBFT)
	@printf "$(BLUE_BOLD)Linking executable '$@'$(RESET)\n"
	@$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
	@printf "$(GREEN)✓ Test '$@' created successfully!$(RESET)\n"

# --- COMPILATION RULES ---
$(OBJ_DIR)/%.o: %.c
	@$(MKDIR) -p $(@D)
	@printf "$(YELLOW)Compiling '$<'$(RESET)\n"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# --- LIBRARY RULES ---
$(LIBFT):
	@printf "$(BLUE_BOLD)Calling make for $@...$(RESET)\n"
	@$(MAKE) -s -C $(LIBFT_DIR)

# --- CLEAN RULES ---
clean:
	@printf "$(YELLOW)Cleaning tests' object files...$(RESET)\n"
	@$(RM) -rf obj
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@printf "$(YELLOW)✓ Object files cleaned.\n$(RESET)"

fclean: clean
	@printf "$(GREEN)Cleaning libraries and executables...$(RESET)\n"
	@$(RM) test_split_quote_expand
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@printf "$(GREEN)✓ Final clean complete.\n$(RESET)"

re:
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) all --no-print-directory

# --- HELPER RULES ---
norm:
	@printf "$(BLUE_BOLD)Checking Norm compliance...$(RESET)\n"
	watch "norminette -R CheckForbiddenSourceHeader $(INC_DIR) src libft | grep Error"

.PHONY: all clean fclean re norm
# $(LIBFT)
# .SECONDARY:
# .DELETE_ON_ERROR:
