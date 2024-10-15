/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:26:45 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 17:31:25 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/finalgnl/get_next_line.h"
# include "../src/lib_FT/libft.h"
# include "builtins.h"
# include "environ.h"
# include "error.h"
# include "executor.h"
# include "memory.h"
# include "parser.h"
# include "path.h"
# include "process.h"
# include "prompt.h"
# include "redirections.h"
# include "signal.h"
# include "signals.h"
# include "structs.h"
# include "tokenizer.h"
# include "utils.h"
# include "var_exp.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <pwd.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

# define MAX_ARGS 1024
# define MAX_REDIRECTS 10
#define MAX_PATH 1024
#define COLOR_GREEN "\001\033[1;32m\002"
#define COLOR_CYAN "\001\033[1;36m\002"
#define COLOR_YELLOW "\001\033[1;33m\002"
#define COLOR_RESET "\001\033[0m\002"


// Main loop
void	minishell_loop(t_shell *shell);
void	free_shell(t_shell *shell);

/* interaction_mode */
int		open_terminal(void);
// char	*get_interactive_input(int terminal_fd, t_shell *shell);
// char	*get_non_interactive_input(void);
void	cleanup(int terminal_fd, int original_stdout);
char	*get_input_line(int terminal_fd, t_shell *shell);

/*minishellception */
char	*ft_itoa(int n);
int		increment_shlvl(t_shell *shell);
void	init_shell(t_shell *shell);

// void	setup_signals(void);

// Memory management
// void        free_command(t_cmd *cmd);

// Error handling
// void        print_error(char *msg);

// environment setup
// char *get_env(const char *name);
// int set_env(const char *name, const char *value);
// int unset_env(const char *name);

// utils
// char *ft_strjoin3(const char *s1, const char *s2, const char *s3);

#endif