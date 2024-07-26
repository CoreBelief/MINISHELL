NAME = minishell
CC = gcc
# CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude -Isrc/lib_FT
LDFLAGS = -lreadline
LIBFT = src/lib_FT/libft.a

SRCS = src/main.c src/executor.c src/tokenizer.c src/signals.c
OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C src/lib_FT

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C src/lib_FT clean
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C src/lib_FT fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re