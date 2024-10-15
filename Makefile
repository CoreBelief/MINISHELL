NAME = minishell
CC = cc -g
CFLAGS = -Wall -Wextra -Werror -Iinclude -Isrc/libft -I/usr/local/opt/readline/include
LDFLAGS = -L/usr/local/opt/readline/lib -lreadline
SRCS_DIR = src
OBJ_DIR = obj

SRCS = $(addprefix $(SRCS_DIR)/, main.c interaction_mode.c prompt/prompt.c prompt/prompt_utils.c error.c signals.c memory.c minishellception.c tokenizer/tokenizer.c \
tokenizer/tokenizer_utils.c tokenizer/tokenizer_word.c tokenizer/var_exp.c tokenizer/var_exp_utils.c parsing/parser.c parsing/parser_utils.c \
parsing/parser_redirects.c parsing/heredoc_parsing.c parsing/heredoc_parsing_utils.c execution/executor.c execution/redirections.c \
execution/process.c execution/path.c builtins/echo.c builtins/cd.c builtins/pwd.c builtins/export.c builtins/export_utils.c builtins/export_utils2.c \
builtins/unset.c builtins/env.c builtins/exit.c builtins/builtins.c environ/environ.c utils/ft_strjoin3.c \
utils/ft_strcmp.c utils/ft_strcpy.c utils/ft_strncpy.c utils/ft_strndup.c finalgnl/get_next_line.c \
finalgnl/get_next_line_utils.c)

OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

OBJ_SUBDIRS = $(sort $(dir $(OBJS)))

LIBFT_DIR = src/lib_FT
LIBFT = $(LIBFT_DIR)/libft.a

# Color codes for pretty output
YELLOW = \033[33m
GREEN = \033[32m
BLUE = \033[34m
RESET = \033[0m

# Nice loading screen with colors
all: $(LIBFT) $(NAME)
	@echo "$(GREEN)Compilation complete!$(RESET)"

$(LIBFT):
	@echo "$(BLUE)Building libft...$(RESET)"
	@make -C $(LIBFT_DIR) > /dev/null
	@echo "$(GREEN)libft built!$(RESET)"

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $@ $(LDFLAGS) > /dev/null
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

# Compile objects with a progress bar
$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo -n "$(YELLOW).$(RESET)"  # Simple loading dot for each file
	@$(CC) $(CFLAGS) -c $< -o $@ > /dev/null

clean:
	@echo "$(BLUE)Cleaning object files...$(RESET)"
	@make -C $(LIBFT_DIR) clean > /dev/null
	@rm -f $(OBJS)
	@rmdir $(OBJ_SUBDIRS) 2>/dev/null || true
	@echo "$(GREEN)Cleaned successfully!$(RESET)"

fclean: clean
	@echo "$(BLUE)Performing full clean...$(RESET)"
	@make -C $(LIBFT_DIR) fclean > /dev/null
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@echo "$(GREEN)Fully cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
