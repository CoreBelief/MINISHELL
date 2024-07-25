// include/builtins.h
#ifndef BUILTINS_H
#define BUILTINS_H

void builtin_cd(char **args);
void builtin_exit(char **args);
void builtin_pwd(char **args);
void builtin_echo(char **args);
void builtin_export(char **args);
void builtin_unset(char **args);
void builtin_env(char **args);

int is_builtin(char *command);
void execute_builtin(char **args);

#endif
