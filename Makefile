# === EXECUTABLE NAME ===
NAME            := minishell

# === CONFIGURATION ===
CC              := cc
RM              := rm -f
MKDIR           := /usr/bin/mkdir -p
MAKE			:= make -L

# === DIRECTORIES ===
# Object file directories
OBJ_DIR_MAND    := obj/mandatory
OBJ_DIR_BONUS   := obj/bonus
# Library & Include directories
LIBFT_DIR       := libft
INC_DIR         := include
# Source directories for vpath
SRC_DIRS        := src src/utils

# === LIBRARIES ===
LIBFT			:= $(LIBFT_DIR)/libft.a

# === FLAGS ===
CFLAGS          := -Wall -Wextra -Werror -MD
CFLAGS			+= -g3 -O0
CPPFLAGS        := -I$(INC_DIR) -I$(LIBFT_DIR)/$(INC_DIR)
LDFLAGS         :=  -L$(LIBFT_DIR)
LDLIBS          :=  -lft -lreadline
## BONUS_FLAG      := -DBONUS_PART=1

# === SOURCE FILES ===
# Use vpath to tell 'make' where to find source files
vpath %.c $(SRC_DIRS)

MANDATORY_SRCS	:= 	minishell.c \
					builtins1.c \
					builtins2.c \
					heredocs.c \
					input.c \
					msh_sqed_functions.c \
					msh_ioredirect.c \
					msh_compile_cmds.c \
					msh_compile_cmds_utils.c \
					msh_compile_cmds_utils2.c \
					paths.c \
					process.c \
					setup_exec.c \
					signals.c \
					utils.c \

BONUS_SRCS		:=  

# === OBJECT FILES ===
# Create separate object lists for each target
OBJS_MANDATORY  := $(addprefix $(OBJ_DIR_MAND)/, $(MANDATORY_SRCS:.c=.o))
OBJS_BONUS      := $(addprefix $(OBJ_DIR_BONUS)/, $(BONUS_SRCS:.c=.o))

# Dependency files for header changes
DEPS            := $(OBJS_MANDATORY:.o=.d) $(OBJS_BONUS:.o=.d)

# --- COLORS ---
GREEN           := \033[0;32m
YELLOW          := \033[0;33m
BLUE_BOLD       := \033[1;34m
RESET           := \033[0m

# === MAIN RULES ===
## .DEFAULT_GOAL   := all
all: mandatory

-include $(DEPS)

# --- TARGETS ---
mandatory: $(NAME)

$(NAME): $(OBJS_MANDATORY) $(LIBFT)
	@printf "$(BLUE_BOLD)Linking '$(NAME)' MANDATORY executable:$(RESET) $(NAME)\n"
	@$(CC) $(OBJS_MANDATORY) -o $(NAME) $(LDFLAGS) $(LDLIBS)
	@printf "$(GREEN)✓ Mandatory project '$(NAME)' created successfully!$(RESET)\n"

#bonus: $(OBJS_BONUS) $(LIBFT)
#	@printf "$(BLUE_BOLD)Linking '$(NAME)' BONUS executable:$(RESET) $(NAME)\n"
#	@$(CC) $(OBJS_BONUS) -o $(NAME) $(LDFLAGS) $(LDLIBS)
#	@printf "$(GREEN)✓ Bonus project '$(NAME)' created successfully!$(RESET)\n"

# --- COMPILATION RULES ---
$(OBJ_DIR_MAND)/%.o: %.c
	@$(MKDIR) -p $(@D)
	@printf "$(YELLOW)Compiling '$(NAME)' [MANDATORY]:$(RESET) $<\n"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

#$(OBJ_DIR_BONUS)/%.o: %.c
#	@$(MKDIR) -p $(@D)
#	@printf "$(YELLOW)Compiling '$(NAME)' [BONUS]:$(RESET) $<\n"
#	@$(CC) $(CPPFLAGS) $(CFLAGS) $(BONUS_FLAG) -c $< -o $@

# --- LIBRARY RULES ---
$(LIBFT):
	@printf "$(BLUE_BOLD)Calling make for $@...$(RESET)\n"
	@$(MAKE) -s -C $(LIBFT_DIR)

# --- CLEAN RULES ---
clean:
	@printf "$(YELLOW)Cleaning project '$(NAME)' object files...$(RESET)\n"
	@$(RM) -rf obj
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@printf "$(YELLOW)✓ Object files cleaned.\n$(RESET)"

fclean: clean
	@printf "$(GREEN)Cleaning '$(NAME)' libraries and executable...$(RESET)\n"
	@$(RM) $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@printf "$(GREEN)✓ Final clean complete.\n$(RESET)"

re:
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) all --no-print-directory

# --- HELPER RULES ---
norm:
	@printf "$(BLUE_BOLD)Checking Norm compliance...$(RESET)\n"
	watch -n1 "norminette -R CheckForbiddenSourceHeader $(INC_DIR) src libft | grep Error"

.PHONY: all bonus mandatory clean fclean re norm
# $(LIBFT)
# .SECONDARY:
# .DELETE_ON_ERROR:
