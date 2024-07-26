NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Isrc/lib_FT
LIBS = -lreadline

SRCS = src/main.c src/executor.c src/tokenizer.c src/signals.c src/parser.c src/redirections.c src/memory.c src/builtins/builtins.c
OBJS = $(SRCS:.c=.o)

LIBFT = src/lib_FT/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C src/lib_FT

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C src/lib_FT clean
	rm -f $(OBJS)

fclean: clean
	make -C src/lib_FT fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re