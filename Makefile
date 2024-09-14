NAME = minishell
CC = gcc -g
CFLAGS =  -Wall -Wextra -Werror  -Iinclude -Isrc/libft -I/usr/local/opt/readline/include 
# this is for mac -I/usr/local/opt/readline/include
LDFLAGS = -g -L/usr/local/opt/readline/lib -lreadline
#and the LDFLAGS is also for mac so can be removed on linux computer
LIBS = -lreadline
SRCS_DIR = src
OBJ_DIR = obj

SRCS = $(addprefix $(SRCS_DIR)/, main.c prompt.c error.c signals.c memory.c parsing/tokenizer.c \
parsing/tokenizer_utils.c parsing/parser.c parsing/parser_utils.c \
execution/executor.c execution/redirections.c execution/process.c execution/path.c builtins/echo.c builtins/cd.c \
builtins/pwd.c builtins/export.c builtins/unset.c builtins/env.c builtins/exit.c \
builtins/builtins.c environ/environ.c parsing/var_exp.c utils/ft_strjoin3.c \
utils/ft_strcmp.c utils/ft_strcpy.c utils/ft_strncpy.c utils/ft_strndup.c)

OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

OBJ_SUBDIRS = $(sort $(dir $(OBJS)))

LIBFT_DIR = src/lib_FT
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $@ $(LIBS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)
	@rmdir $(OBJ_SUBDIRS) 2>/dev/null || true

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re