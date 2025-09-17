# === EXECUTABLE NAME ===
#NAME            := <div testing executables>

# === CONFIGURATION ===
CC              := cc
RM              := rm
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

# === OBJECT FILES ===
# Create separate object lists for each target
OBJFILES  := $(addprefix $(OBJ_DIR)/, $(SRCFILES:.c=.o))

# Dependency files for header changes
DEPS            := $(OBJFILES:.o=.d)

# --- COLORS ---
GREEN           := \033[0;32m
YELLOW          := \033[0;33m
BLUE_BOLD       := \033[1;34m
RESET           := \033[0m

# === MAIN RULES ===
## .DEFAULT_GOAL   := all
all:
	@printf "$(YELLOW)Choose your test... (as param to Makefile)$(RESET)\n"
	@printf "$(YELLOW)or simply run Debug session '(gdb) Run test program' from within code$(RESET)\n"

-include $(DEPS)

COMMON_RECIPE =\
	@printf "$(BLUE_BOLD)Linking executable '$@'$(RESET)\n" ;\
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS) ;\
	printf "$(GREEN)✓ Test '$@' created successfully!$(RESET)\n" ;\

OBJFILES = $(addprefix $(OBJ_DIR)/, $(SRCFILES:.c=.o))
COMMON_DEPS = $(OBJFILES) $(LIBFT)
COMMON_INC = $(OBJFILES:.o=.d)
#$(info $$var is [${OBJFILES:.o=.d}])

# --- TARGETS ---
SRCFILES	:= test_quote_expand.c ms_rmquotes_expand.c
-include $(COMMON_INC)
test_quote_expand: $(COMMON_DEPS)
	$(COMMON_RECIPE)

SRCFILES	:= test_split.c
-include $(COMMON_INC)
test_split: $(COMMON_DEPS)
	$(COMMON_RECIPE)

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
	@$(RM) -i test_*
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@printf "$(GREEN)✓ Final clean complete.\n$(RESET)"

# re:
# 	@$(MAKE) fclean --no-print-directory
# 	@$(MAKE) all --no-print-directory

# --- HELPER RULES ---
norm:
	@printf "$(BLUE_BOLD)Checking Norm compliance...$(RESET)\n"
	watch "norminette -R CheckForbiddenSourceHeader $(INC_DIR) src libft | grep Error"

.PHONY: all clean fclean re norm
# $(LIBFT)
# .SECONDARY:
# .DELETE_ON_ERROR:
