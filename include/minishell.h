/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:26:45 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/13 03:23:27 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "environ.h"
#include "signal.h"
#include "../src/lib_FT/libft.h"
#include "prompt.h"
# include "structs.h"
# include "builtins.h"
# include "parser.h"
# include "tokenizer.h"
# include "memory.h"
# include "executor.h"
# include "var_exp.h"
# include "environ.h"
#include "signals.h"

# define MAX_ARGS 1024
# define MAX_REDIRECTS 10

// Global variables
extern int g_exit_status;
extern char **environ;

// Function prototypes

// Main loop
void    minishell_loop(t_shell *shell);

void	setup_signals(void);
// Environment variables
char        *expand_variables(char *str);

// Redirections and pipes
void        setup_redirections(t_command *cmd);
void        handle_pipes(t_command *cmd1, t_command *cmd2);

// Memory management
// void        free_command(t_command *cmd);

// Error handling
void        print_error(char *msg);

// environment setup
char *get_env(const char *name);
int set_env(const char *name, const char *value);
int unset_env(const char *name);

//utils
// char *ft_strjoin3(const char *s1, const char *s2, const char *s3);

#endif