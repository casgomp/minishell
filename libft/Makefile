# === EXECUTABLE NAME ===
NAME            := libft.a

# === CONFIGURATION ===
CC              := cc
CP              := cp
AR              := ar rcs
RM              := rm -f
MKDIR           := mkdir -p

# === DIRECTORIES ===
# Object file directories
OBJ_DIR    := obj
# Library & Include directories
#LIBFT_DIR       := libft
INC_DIR         := include
# Source directories for vpath
SRC_DIRS        := src

# === LIBRARIES ===
#LIBFT           := $(LIBFT_DIR)/libft.a

# === FLAGS ===
CFLAGS          := -Wall -Wextra -Werror -g3 -O0 -MD
CPPFLAGS        := -I$(INC_DIR)  # -I$(LIBFT_DIR)/$(INC_DIR)
## LDFLAGS         := -L$(LIBFT_DIR)
## LDLIBS          := -lft
## BONUS_FLAG      := -DBONUS_PART=1

# === SOURCE FILES ===
# Use vpath to tell 'make' where to find source files
vpath %.c $(SRC_DIRS)

# Core Libft
MANDATORY_SRCS	:= \
	ft_isalpha.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_isascii.c \
	ft_isprint.c \
	ft_strlen.c \
	ft_memset.c \
	ft_bzero.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_toupper.c \
	ft_tolower.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strncmp.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_strnstr.c \
	ft_atoi.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_split.c \
	ft_split_quotes.c \
	ft_split_quotes2.c \
	ft_itoa.c \
	ft_itoa_base.c \
	ft_strmapi.c \
	ft_striteri.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_strdup.c \
	ft_calloc.c \

# ft_printf
MANDATORY_SRCS	+= \
	ft_printf.c \
	ftprintf_fmtconv_std.c \
	ftprintf_output.c \
	ftprintf_fmtspec.c \
	ftprintf_write.c \
	ftprintf_fmtconv_xtra.c

# get_next_line
MANDATORY_SRCS	+= \
	get_next_line.c\
	get_next_line_utils.c

MANDATORY_SRCS	+= \
	ft_lstnew.c \
	ft_lstadd_front.c \
	ft_lstsize.c \
	ft_lstadd_back.c \
	ft_lstlast.c \
	ft_lstdelone.c \
	ft_lstclear.c \
	ft_lstiter.c \
	ft_lstmap.c

# === OBJECT FILES ===
# Create separate object lists for each target
OBJS  := $(addprefix $(OBJ_DIR)/, $(MANDATORY_SRCS:.c=.o))

# Dependency files for header changes
DEPS            := $(OBJS:.o=.d) $(OBJS_BONUS:.o=.d)

# --- COLORS ---
GREEN           := \033[0;32m
YELLOW          := \033[0;33m
BLUE_BOLD       := \033[1;34m
RESET           := \033[0m

# === MAIN RULES ===
## .DEFAULT_GOAL   := all
all: $(NAME)

-include $(DEPS)

# --- TARGETS ---

$(NAME): $(OBJS)
	@printf "$(BLUE_BOLD)Linking MANDATORY library:$(RESET) $(NAME)\n"
	@$(AR) $(NAME) $(OBJS)
	@printf "$(GREEN)✓ Mandatory project '$(NAME)' created successfully!$(RESET)\n"

# --- COMPILATION RULES ---
$(OBJ_DIR)/%.o: %.c
	@$(MKDIR) -p $(@D)
	@printf "$(YELLOW)Compiling '$(NAME)' [MANDATORY]:$(RESET) $<\n"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# --- CLEAN RULES ---
clean:
	@printf "$(YELLOW)Cleaning project '$(NAME)' object files...$(RESET)\n"
	@$(RM) -rf obj
	@printf "$(YELLOW)✓ Object files cleaned.\n$(RESET)"

fclean: clean
	@printf "$(GREEN)Cleaning '$(NAME)' libraries and executable...$(RESET)\n"
	@$(RM) -f $(NAME)
	@printf "$(GREEN)✓ Final clean complete.\n$(RESET)"

re:
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) all --no-print-directory

# --- HELPER RULES ---
norm:
	@printf "$(BLUE_BOLD)Checking Norm compliance...$(RESET)\n"
#	@norminette -R CheckForbiddenSourceHeader $(INC_DIR) $(SRC_DIRS)
	watch "norminette -R CheckForbiddenSourceHeader $(INC_DIR) $(SRC_DIRS) | grep Error"

.PHONY: all bonus mandatory clean fclean re norm
# .SECONDARY:
# .DELETE_ON_ERROR:
