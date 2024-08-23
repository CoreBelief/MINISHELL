#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

// Builtin function prototypes
int     is_builtin(char *command);
void    execute_builtin(t_command *cmd);
void    builtin_echo(char **args);
void    builtin_cd(char **args);
void    builtin_pwd(char **args);
void    builtin_export(char **args);
void    builtin_unset(char **args);
void    builtin_env(char **args);
void    builtin_exit(char **args);
int is_parent_command(char *command);
void    execute_external(t_command *cmd);
void execute_parent(t_command *cmd);
int    is_builtin_parent(char *command);



#endif