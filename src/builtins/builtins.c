#include "builtins.h"
#include "minishell.h"
#include <string.h>
#include <stdio.h>

// Array of builtin command names
static const char *builtin_commands[] = {
    "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
};

// Function prototypes for builtin commands
// void builtin_echo(char **args);
// void builtin_cd(char **args);
// void builtin_pwd(char **args);
// void builtin_export(char **args);
// void builtin_unset(char **args);
// void builtin_env(char **args);
// void builtin_exit(char **args);

int is_builtin(char *command)
{
    // printf("DEBUG: Checking if '%s' is a builtin command\n", command);
    int i = 0;
    while (builtin_commands[i])
    {
        // printf("DEBUG: Comparing '%s' with '%s'\n", command, builtin_commands[i]);
        if (strcmp(command, builtin_commands[i]) == 0)
        {
            // printf("DEBUG: Found builtin command '%s'\n", command);
            return 1;
        }
        i++;
    }
    // printf("DEBUG: '%s' is not a builtin command\n", command);
    return 0;
}

void execute_builtin(t_command *cmd)
{
    // printf("DEBUG: Executing builtin command '%s'\n", cmd->argv[0]);
    if (strcmp(cmd->argv[0], "echo") == 0)
    {
        // printf("DEBUG: Executing 'echo' builtin\n");
        builtin_echo(cmd->argv);
    }
    else if (strcmp(cmd->argv[0], "cd") == 0)
    {
        // printf("DEBUG: Executing 'cd' builtin\n");
        builtin_cd(cmd->argv);
    }
    else if (strcmp(cmd->argv[0], "pwd") == 0)
    {
        // printf("DEBUG: Executing 'pwd' builtin\n");
        builtin_pwd(cmd->argv);
    }
    else if (strcmp(cmd->argv[0], "export") == 0)
    {
        // printf("DEBUG: Executing 'export' builtin\n");
        builtin_export(cmd->argv);
    }
    else if (strcmp(cmd->argv[0], "unset") == 0)
    {
        // printf("DEBUG: Executing 'unset' builtin\n");
        builtin_unset(cmd->argv);
    }
    else if (strcmp(cmd->argv[0], "env") == 0)
    {
        // printf("DEBUG: Executing 'env' builtin\n");
        builtin_env(cmd->argv);
    }
    else if (strcmp(cmd->argv[0], "exit") == 0)
    {
        // printf("DEBUG: Executing 'exit' builtin\n");
        builtin_exit(cmd->argv);
    }
    // printf("DEBUG: Finished executing builtin command '%s'\n", cmd->argv[0]);
}