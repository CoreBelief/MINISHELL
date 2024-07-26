#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void execute_command(char *command)
{
    t_command *cmd;
    pid_t pid;
    int status;

    cmd = parse_command(command);
    if (!cmd)
        return;
    
    if (is_builtin(cmd->argv[0]))
    {
        execute_builtin(cmd);
    }
    else
    {
        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
        }
        else if (pid == 0)
        {
            setup_redirections(cmd);
            if (execvp(cmd->argv[0], cmd->argv) == -1)
            {
                perror("minishell: execvp failed");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
            update_exit_status(status);
        }
    }
    
    free_command(cmd);  // Make sure this is called only once
}