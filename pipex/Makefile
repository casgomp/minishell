CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0 -Iinclude -Ilib/libft/include
MAKEFLAGS += --no-print-directory
NAME = pipex

SRC_DIR = src
OBJ_DIR = objs
LIBFT_DIR = lib/libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SRCS = main.c \
	   ft_paths.c \
	   ft_processes.c \
	   ft_clean.c

OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

vpath %.c $(SRC_DIR)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -o $(NAME)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
