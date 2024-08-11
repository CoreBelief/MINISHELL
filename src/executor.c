#include "minishell.h"

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_command *parse_command_from_tokens(t_token *tokens);
// char *find_executable(char *command)
// {
//     char *path = getenv("PATH");
//     char *path_copy = ft_strdup(path);
//     //illegal strtok!!!
//     char *dir = strtok(path_copy, ":");
//     char *full_path = NULL;

//     while (dir != NULL) {
//         full_path = malloc(ft_strlen(dir) + ft_strlen(command) + 2);
//         sprintf(full_path, "%s/%s", dir, command);
//         if (access(full_path, X_OK) == 0) {
//             free(path_copy);
//             return full_path;
//         }
//         free(full_path);
//         dir = strtok(NULL, ":");
//     }

//     free(path_copy);
//     return NULL;
// }
char *find_executable(char *command)
{
    char *path = getenv("PATH");
    char *path_copy = ft_strdup(path);
    char *dir = strtok(path_copy, ":");
    char *full_path = NULL;

    while (dir != NULL) {
        full_path = ft_strjoin3(dir, "/", command);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return ft_strdup(command);  // Return the command as-is if not found in PATH
}

// void execute_command(char *command)
// {
//     t_command *cmd;
//     pid_t pid;
//     int status;

//     cmd = parse_command(command);
//     if (!cmd)
//         return;

//     if (is_builtin(cmd->argv[0]))
//     {
//         printf("Executing builtin command: %s\n", cmd->argv[0]);
//         execute_builtin(cmd);
//     }
//     else
//     {
//         pid = fork();
//         if (pid == -1)
//         {
//             perror("minishell: fork");
//         }
//         else if (pid == 0)
//         {
//             setup_redirections(cmd);
//             char *executable_path = find_executable(cmd->argv[0]);
//             if (executable_path)
//             {   
//                 printf("Executing external command: %s\n", executable_path);
//                 if (execve(executable_path, cmd->argv, environ) == -1)
//                 {
//                     perror("minishell: execve failed");
//                     free(executable_path);
//                     exit(EXIT_FAILURE);
//                 }
//             }
//             else
//             {
//                 fprintf(stderr, "minishell: command not found: %s\n", cmd->argv[0]);
//                 exit(EXIT_FAILURE);
//             }
//         }
//         else
//         {
//             waitpid(pid, &status, 0);
//             update_exit_status(status);
//         }
//     }

//     free_command(cmd);  // Make sure this is called only once
// // }
// void execute_command(char *command)
// {
//     t_command *cmd;
//     pid_t pid;
//     int status;

//     cmd = parse_command(command);
//     if (!cmd)
//         return;

//     if (is_builtin(cmd->argv[0]))
//     {
//         execute_builtin(cmd);
//     }
//     else
//     {
//         pid = fork();
//         if (pid == -1)
//         {
//             perror("minishell: fork");
//         }
//         else if (pid == 0)
//         {
//             setup_redirections(cmd);
//             char *executable_path = find_executable(cmd->argv[0]);
//             if (executable_path)
//             {
//                 execve(executable_path, cmd->argv, environ);
//                 perror("minishell: execve failed");
//                 free(executable_path);
//                 exit(EXIT_FAILURE);
//             }
//             else
//             {
//                 fprintf(stderr, "minishell: command not found: %s\n", cmd->argv[0]);
//                 exit(EXIT_FAILURE);
//             }
//         }
//         else
//         {
//             waitpid(pid, &status, 0);
//             update_exit_status(status);
//         }
//     }

//     free_command(cmd);
// }
void execute_command(char *command)
{
    t_token *tokens;
    t_command *cmd;
    pid_t pid;
    int status;
    printf("command: %s\n", command);
    tokens = tokenizer(command);
    print_token_list(tokens);
    if (!tokens)
        return;

    cmd = parse_command_from_tokens(tokens);
    if (!cmd)
    {
        free_tokens(&tokens);
        return;
    }

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
            execvp(cmd->argv[0], cmd->argv);
            perror("minishell: execvp failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
            update_exit_status(status);
        }
    }

    free_command(cmd);
    free_tokens(&tokens);
}
t_command *parse_command_from_tokens(t_token *tokens)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

    cmd->argv = malloc(sizeof(char *) * (MAX_ARGS + 1));
    if (!cmd->argv)
    {
        free(cmd);
        return NULL;
    }

    int i = 0;
    while (tokens && i < MAX_ARGS)
    {
        if (tokens->type == TOKEN_WORD)
        {
            cmd->argv[i] = ft_strdup(tokens->content);
            i++;
        }
        // Handle redirections and other token types here
        tokens = tokens->next;
    }
    cmd->argv[i] = NULL;

    cmd->path = ft_strdup(cmd->argv[0]);
    cmd->redirects = NULL;
    cmd->redirect_count = 0;
    cmd->pipe_in = STDIN_FILENO;
    cmd->pipe_out = STDOUT_FILENO;

    return cmd;
}