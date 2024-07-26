# Minishell Makefile

NAME = minishell
CC = cc
# CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

# Libft
LIBFT_DIR = lib_FT
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_INC = -I$(LIBFT_DIR)

# Minishell source files
SRC = main.c executor.c tokenizer.c

# Object files
OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Include directories
INC_DIR = .
INC = -I$(INC_DIR) $(LIBFT_INC)

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) -L$(LIBFT_DIR) -lft

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

bonus: all

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)
	find . -name "*.o" -type f -delete

re: fclean all