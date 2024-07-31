NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Isrc/lib_FT
LIBS = -lreadline

SRCS = src/main.c src/executor.c src/tokenizer.c src/signals.c src/parser.c src/redirections.c src/memory.c src/builtins/builtins.c src/environ/environ.c \
src/utils/ft_strjoin3.c src/builtins/echo.c src/builtins/cd.c src/builtins/pwd.c src/builtins/export.c src/builtins/unset.c src/builtins/env.c src/builtins/exit.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = src/lib_FT
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re