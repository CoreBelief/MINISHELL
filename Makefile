# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC = src/main.c src/signal_handlers.c src/prompt.c src/tokenizer.c src/parser.c \
      src/executor.c src/redirections.c src/env.c src/memory.c src/error.c \
      src/builtins/cd.c src/builtins/exit.c src/builtins/pwd.c src/builtins/echo.c \
      src/builtins/export.c src/builtins/unset.c src/builtins/env.c
OBJ = $(SRC:.c=.o)
INCLUDE = -Iinclude
NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
