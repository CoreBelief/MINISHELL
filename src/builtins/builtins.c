#include "builtins.h"
#include "minishell.h"

int is_builtin(char *command)
{
    // Implement this function
    (void)command;
    return 0;
}

void execute_builtin(t_command *cmd)
{
    // Implement this function
    (void)cmd;
}

void builtin_echo(t_command *cmd)
{
    // Implement echo builtin
    (void)cmd;
}

void builtin_cd(t_command *cmd)
{
    // Implement cd builtin
    (void)cmd;
}

void builtin_pwd(void)
{
    // Implement pwd builtin
}

void builtin_export(t_command *cmd)
{
    // Implement export builtin
    (void)cmd;
}

void builtin_unset(t_command *cmd)
{
    // Implement unset builtin
    (void)cmd;
}

void builtin_env(void)
{
    // Implement env builtin
}

void builtin_exit(t_command *cmd)
{
    // Implement exit builtin
    (void)cmd;
}