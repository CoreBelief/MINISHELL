// src/executor.c
#include "executor.h"
#include "builtins.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void execute_command(command_t *cmd) {
    if (is_builtin(cmd->args[0])) {
        execute_builtin(cmd->args);
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (execvp(cmd->args[0], cmd->args) == -1) {
                perror("minishell");
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Error forking
            perror("minishell");
        } else {
            // Parent process
            waitpid(pid, NULL, 0);
        }
    }
}
