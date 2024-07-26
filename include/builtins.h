#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

// Builtin function prototypes
int         is_builtin(char *command);
void        execute_builtin(t_command *cmd);
void        builtin_echo(t_command *cmd);
void        builtin_cd(t_command *cmd);
void        builtin_pwd(void);
void        builtin_export(t_command *cmd);
void        builtin_unset(t_command *cmd);
void        builtin_env(void);
void        builtin_exit(t_command *cmd);

#endif