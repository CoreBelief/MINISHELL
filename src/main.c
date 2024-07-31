#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "environ.h"

int g_exit_status = 0;

void update_exit_status(int status)
{
    printf("DEBUG: Updating exit status\n");
    if (WIFEXITED(status))
    {
        g_exit_status = WEXITSTATUS(status);
        printf("DEBUG: Process exited normally with status %d\n", g_exit_status);
    }
    else if (WIFSIGNALED(status))
    {
        g_exit_status = 128 + WTERMSIG(status);
        printf("DEBUG: Process terminated by signal %d\n", WTERMSIG(status));
    }
}

void minishell_loop(void)
{
    char *line;

    printf("DEBUG: Setting up signals\n");
    setup_signals();

    while (1)
    {
        printf("DEBUG: Minishell PID: %d\n", getpid());
        line = readline("minishell> ");
        if (!line)
        {
            printf("DEBUG: EOF detected, exiting\n");
            printf("exit\n");
            break;
        }

        if (*line)
        {
            printf("DEBUG: Adding command to history: %s\n", line);
            add_history(line);
            printf("DEBUG: Executing command: %s\n", line);
            execute_command(line);
        }

        free(line);
        printf("DEBUG: Command execution complete\n");
    }
}

int main(void)
{
    printf("DEBUG: Starting Minishell\n");
    
    printf("DEBUG: Entering main shell loop\n");
    minishell_loop();
    
    printf("DEBUG: Exiting Minishell with status: %d\n", g_exit_status);
    return (g_exit_status);
}