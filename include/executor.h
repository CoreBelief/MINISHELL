
#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"

// void	execute_command(char *command);

void		execute_command(t_shell *shell);
void		update_exit_status(int status, t_shell *shell);

#endif