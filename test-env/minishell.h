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


# define MAX_ARGS 64

extern int	g_exit_status;

void	minishell_loop(void);
void	execute_command(char *command);

#endif