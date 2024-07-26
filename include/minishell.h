#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <signal.h>
# include "tokenizer.h"
# include "executor.h"
#include "signals.h"


# define MAX_ARGS 64

extern int	g_exit_status;

void	minishell_loop(void);
void	execute_command(char *command);

// Signal handling
void    setup_signals(void);
void    handle_sigint(int sig);
void    handle_sigquit(int sig);

#endif