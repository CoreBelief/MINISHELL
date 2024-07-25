// src/builtins/builtins.c
#include "builtins.h"
#include <string.h>

int is_builtin(char *command) {
    return (
        strcmp(command, "cd") == 0 ||
        strcmp(command, "exit") == 0 ||
        strcmp(command, "pwd") == 0 ||
        strcmp(command, "echo") == 0 ||
        strcmp(command, "export") == 0 ||
        strcmp(command, "unset") == 0 ||
        strcmp(command, "env") == 0
    );
}

void execute_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        builtin_cd(args);
    } else if (strcmp(args[0], "exit") == 0) {
        builtin_exit(args);
    } else if (strcmp(args[0], "pwd") == 0) {
        builtin_pwd(args);
    } else if (strcmp(args[0], "echo") == 0) {
        builtin_echo(args);
    } else if (strcmp(args[0], "export") == 0) {
        builtin_export(args);
    } else if (strcmp(args[0], "unset") == 0) {
        builtin_unset(args);
    } else if (strcmp(args[0], "env") == 0) {
        builtin_env(args);
    }
}
