/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:26:45 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/08 15:18:57 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/finalgnl/get_next_line.h"

# include "utils.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <stdbool.h>
# include <pwd.h>
# include <errno.h>
# include <sys/stat.h>

# include "environ.h"
# include "signal.h"
# include "../src/lib_FT/libft.h"
# include "prompt.h"
# include "structs.h"
# include "builtins.h"
# include "parser.h"
# include "tokenizer.h"
# include "memory.h"
# include "executor.h"
# include "var_exp.h"
# include "signals.h"
# include "redirections.h"
# include "error.h"
# include "path.h"
# include "utils.h"
# include "process.h"

# define MAX_ARGS 1024
# define MAX_REDIRECTS 10

// Main loop
void	minishell_loop(t_shell *shell);

// void	setup_signals(void);

// Memory management
// void        free_command(t_cmd *cmd);

// Error handling
// void        print_error(char *msg);

// environment setup
// char *get_env(const char *name);
// int set_env(const char *name, const char *value);
// int unset_env(const char *name);

//utils
// char *ft_strjoin3(const char *s1, const char *s2, const char *s3);

#endif