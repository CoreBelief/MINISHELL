NAME = minishell
CC = gcc -g
CFLAGS = -Wall -Wextra -Werror -Iinclude -Isrc/lib_FT -I/usr/local/opt/readline/include
# this is for mac -I/usr/local/opt/readline/include
LDFLAGS = -g -L/usr/local/opt/readline/lib -lreadline
#and the LDFLAGS is also for mac so can be removed on linux computer
LIBS = -lreadline

SRCS = src/main.c src/executor.c src/tokenizer.c src/tokenizer_utils.c src/signals.c src/parser.c src/parser_utils.c src/redirections.c src/memory.c src/builtins/builtins.c src/environ/environ.c src/var_exp.c \
src/utils/ft_strjoin3.c src/utils/ft_strcmp.c src/utils/ft_strcpy.c src/utils/ft_strncpy.c src/utils/ft_strndup.c src/builtins/echo.c src/builtins/cd.c src/builtins/pwd.c src/builtins/export.c src/builtins/unset.c src/builtins/env.c src/builtins/exit.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = src/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $@ $(LIBS) $(LDFLAGS)

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