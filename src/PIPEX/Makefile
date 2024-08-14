# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: skoene <skoene@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2024/04/26 14:14:05 by skoene        #+#    #+#                  #
#    Updated: 2024/05/25 15:26:42 by skoene        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = pipex
HEADER = pipex.h
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR)
OBJDIR = obj/

# Color Codes
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

SRC = \
	pipex.c \
	processes.c \
	pipex_utils.c \
	free.c \

OBJ = $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR),$(OBJ))

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "$(YELLOW)Compiling $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)Compilation done!$(NC)"

$(OBJDIR)%.o: %.c $(HEADER)
	@mkdir -p $(OBJDIR)
	@$(CC) -c $(CFLAGS) $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "$(YELLOW)Cleaning object files...$(NC)"
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJDIR)
	@echo "$(GREEN)Object files cleaned.$(NC)"

fclean: clean
	@echo "$(YELLOW)Starting full clean..."
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(YELLOW)Removing executable $(NAME)..."
	@rm -f $(NAME)
	@echo "$(GREEN)Full clean completed."

re: fclean all
	@echo "$(GREEN)Recompiled everything.$(NC)"

.PHONY: all clean fclean re