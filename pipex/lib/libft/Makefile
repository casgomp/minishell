CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
NAME = libft.a

OBJ_DIR = objs
SRC_DIR = src
SUB_DIRS = ctype string memory alloc function fd linked_list printf dprintf gnl

S_CTYPE			= ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
				  ft_isprint.c ft_tolower.c ft_toupper.c

S_STRING		= ft_strchr.c ft_strrchr.c ft_strnstr.c ft_strcmp.c \
				  ft_strncmp.c ft_atoi.c ft_atoi_safe.c ft_strlcat.c \
				  ft_strlcpy.c ft_strlen.c

S_MEMORY		= ft_memchr.c ft_memcpy.c ft_memmove.c ft_memset.c \
				  ft_bzero.c ft_memcmp.c

S_ALLOC			= ft_calloc.c ft_itoa.c ft_strdup.c ft_split.c \
				  ft_strjoin.c ft_strtrim.c ft_substr.c

S_FD			= ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
				  ft_putstr_fd.c

S_FUNCTION		= ft_striteri.c ft_strmapi.c

S_LINKED_LIST	= ft_lstnew.c ft_lstlast.c ft_lstmap.c ft_lstadd_front.c \
				  ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c \
				  ft_lstiter.c ft_lstsize.c

S_PRINTF		= ft_printf.c ft_putchar_prnt.c ft_putstr_prnt.c \
				  ft_putnbr_base_prnt.c ft_putnbr_base_prnt_address.c \
				  ft_putnbr_base_prnt_address_pre.c
				  
S_DPRINTF		= ft_dprintf.c ft_putchar_dprintf.c ft_putstr_dprintf.c \
				  ft_putnbr_dprintf.c ft_putnbr_base_unsigned_dprintf.c

S_GNL			= ft_get_next_line.c ft_get_next_line_utils.c

O_CTYPE			= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_CTYPE))
O_STRING		= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_STRING))
O_MEMORY		= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_MEMORY))
O_ALLOC			= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_ALLOC))
O_FD			= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_FD))
O_FUNCTION		= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_FUNCTION))
O_LINKED_LIST	= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_LINKED_LIST))
O_PRINTF		= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_PRINTF))
O_DPRINTF		= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_DPRINTF))
O_GNL			= $(patsubst %.c, $(OBJ_DIR)/%.o, $(S_GNL))

OBJS =  $(O_CTYPE) $(O_STRING) $(O_MEMORY) $(O_ALLOC) $(O_FD) \
		$(O_FUNCTION) $(O_LINKED_LIST) $(O_PRINTF) $(O_DPRINTF) \
		$(O_GNL)

vpath %.c $(addprefix $(SRC_DIR)/, $(SUB_DIRS))

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "Created $(NAME) with all object files."

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< -> $@"

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
