#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

// Function to handle child process execution
static void child_process(char **args)
{
    printf("DEBUG: Entering child process\n");
    printf("external: %s\n", args[0]);
    
    // Attempt to execute the command
    if (execvp(args[0], args) == -1) {
        perror("minishell: execvp failed");
        printf("DEBUG: Child process execution failed\n");
        exit(EXIT_FAILURE);
    }
    // Note: If execvp is successful, the code below will not be reached
}

// Function to check if a command is a builtin
static int is_builtin(char *cmd)
{
    printf("DEBUG: Checking if '%s' is a builtin\n", cmd);
    // This is a placeholder. Implement actual builtin check.
    // For example: return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "echo") == 0);
    return (0);
}

// Function to execute builtin commands
static void execute_builtin(char **args)
{
    printf("DEBUG: Executing builtin command '%s'\n", args[0]);
    // This is a placeholder. Implement actual builtin execution.
    // For example:
    // if (strcmp(args[0], "cd") == 0) {
    //     // Implement cd
    // } else if (strcmp(args[0], "echo") == 0) {
    //     // Implement echo
    // }
    printf("internal: %s\n", args[0]);
}

// Main function to execute a command
void execute_command(char *command)
{
    pid_t pid;
    int status;
    char **args;

    printf("DEBUG: Executing command: '%s'\n", command);

    args = tokenize_command(command);
    if (!args) {
        printf("DEBUG: Command tokenization failed\n");
        return;
    }

    if (is_builtin(args[0])) {
        execute_builtin(args);
    } else {
        printf("DEBUG: Forking process to execute external command\n");
        pid = fork();
        if (pid == -1) {
            perror("minishell: fork failed");
        } else if (pid == 0) {
            child_process(args);
        } else {
            printf("DEBUG: Parent process waiting for child (PID: %d)\n", pid);
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                g_exit_status = WEXITSTATUS(status);
                printf("DEBUG: Child process exited with status %d\n", g_exit_status);
            } else if (WIFSIGNALED(status)) {
                printf("DEBUG: Child process terminated by signal %d\n", WTERMSIG(status));
            }
        }
    }

    printf("DEBUG: Freeing argument array\n");
    free(args);
    printf("DEBUG: Command execution completed\n");
}