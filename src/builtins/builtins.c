#include "builtins.h"
#include "minishell.h"
#include <string.h>
#include <stdio.h>



//rework parent process to handle builtins and externals
// # Commands that must be executed in the parent process:

// 1. cd (change directory)
// 2. export (set environment variables)
// 3. unset (unset environment variables)
// 4. exit (terminate the shell)
// 5. source or . (execute commands from a file in the current shell)
// 6. alias (define or display aliases)
// 7. unalias (remove aliases)
// 8. set (set or unset shell options and positional parameters)
// 9. jobs (display status of jobs in the current session)
// 10. fg (move job to foreground)
// 11. bg (move job to background)

char *find_executable(char *command);

static const char *parent_commands[] = {
    "cd", "export", "unset", "exit", "source", ".", "alias", "unalias", "set", "jobs", "fg", "bg", NULL
};

static const char *builtin_parent_commands[] = {
    "cd", "export", "unset", "exit", NULL
};
// Array of builtin command names
static const char *builtin_commands[] = {
    "cd", "pwd", "export", "echo", "unset", "env", "exit", "bye", NULL
};

// static const char *builtin_child_commands[] = {
//     "pwd", "echo", "env", NULL
// };

int is_parent_command(char *command)
{
    // printf("DEBUG: Checking if '%s' is a builtin command\n", command);
    int i = 0;
    while (parent_commands[i])
    {
        // printf("DEBUG: Comparing '%s' with '%s'\n", command, builtin_commands[i]);
        if (ft_strcmp(command, parent_commands[i]) == 0)
        {
            // printf("DEBUG: Found builtin command '%s'\n", command);
            return 1;
        }
        i++;
    }
    // printf("DEBUG: '%s' is not a builtin command\n", command);
    return 0;
}

int    is_builtin_parent(char *command)
{
    // printf("DEBUG: Checking if '%s' is a builtin command\n", command);
    int i = 0;
    while (builtin_parent_commands[i])
    {
        // printf("DEBUG: Comparing '%s' with '%s'\n", command, builtin_commands[i]);
        if (ft_strcmp(command, builtin_parent_commands[i]) == 0)
        {
            // printf("DEBUG: Found builtin command '%s'\n", command);
            return 1;
        }
        i++;
    }
    // printf("DEBUG: '%s' is not a builtin command\n", command);
    return 0;
}

void    execute_external(t_command *cmd)
{
    char *path;

    path = find_executable(cmd->argv[0]);
    printf("external command: %s\n", path);
    execve(path, cmd->argv, environ);
    perror("minishell: execve failed\n");
    exit(EXIT_FAILURE);
}
void execute_parent(t_command *cmd)
{
    if (is_builtin_parent(cmd->argv[0]))
    {
        execute_builtin(cmd);
    }
    else
    {
        execute_external(cmd);
    }
}

int is_builtin(char *command)
{
    // printf("DEBUG: Checking if '%s' is a builtin command\n", command);
    int i = 0;
    while (builtin_commands[i])
    {
        // printf("DEBUG: Comparing '%s' with '%s'\n", command, builtin_commands[i]);
        if (ft_strcmp(command, builtin_commands[i]) == 0)
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
    if (ft_strcmp(cmd->argv[0], "echo") == 0)
    {
        // printf("DEBUG: Executing 'echo' builtin\n");
        builtin_echo(cmd->argv);
    }
    else if (ft_strcmp(cmd->argv[0], "cd") == 0)
    {
        // printf("DEBUG: Executing 'cd' builtin\n");
        builtin_cd(cmd->argv);
    }
    else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
    {
        // printf("DEBUG: Executing 'pwd' builtin\n");
        builtin_pwd(cmd->argv);
    }
    else if (ft_strcmp(cmd->argv[0], "export") == 0)
    {
        // printf("DEBUG: Executing 'export' builtin\n");
        builtin_export(cmd->argv);
    }
    else if (ft_strcmp(cmd->argv[0], "unset") == 0)
    {
        // printf("DEBUG: Executing 'unset' builtin\n");
        builtin_unset(cmd->argv);
    }
    else if (ft_strcmp(cmd->argv[0], "env") == 0)
    {
        // printf("DEBUG: Executing 'env' builtin\n");
        builtin_env(cmd->argv);
    }
    else if (ft_strcmp(cmd->argv[0], "exit") == 0)
    {
        // printf("DEBUG: Executing 'exit' builtin\n");
        builtin_exit(cmd->argv);
    }
     else if (ft_strcmp(cmd->argv[0], "bye") == 0)
    {
        // printf("DEBUG: Executing 'exit' builtin\n");
        builtin_exit(cmd->argv);
    }
    // printf("DEBUG: Finished executing builtin command '%s'\n", cmd->argv[0]);
}