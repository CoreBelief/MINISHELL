#include "minishell.h"

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void	print_cmd_list(t_command *head);

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
    // print_cmd_list(cmd);

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

t_command	*init_cmd(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *) * (MAX_ARGS + 1));
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->redirects = malloc (sizeof(t_redirect) * MAX_REDIRECTS);
	if (!cmd->argv)
	{
		free(cmd->argv);
		free(cmd);
		return (NULL);
	}
	cmd->redirect_count = 0;
	cmd->pipe_in = STDIN_FILENO;
	cmd->pipe_out = STDOUT_FILENO;
	cmd->next = NULL;
	cmd->path = NULL;
	return (cmd);
}

// void	free_command_list(t_command *head)

int	is_redirect_token(int token_type)
{
	return (token_type == TOKEN_REDIRECT_IN || \
		token_type == TOKEN_REDIRECT_OUT || \
		token_type == TOKEN_REDIRECT_APPEND || \
		token_type == TOKEN_HEREDOC);
}

int	handle_redirection_parsing(t_command *cmd, t_token **token)
{
	cmd->redirects[cmd->redirect_count].type = (*token)->type;
	token = (*token)->next;
	if (token && (*token)->type == TOKEN_WORD)
	{
		cmd->redirects[cmd->redirect_count].file = ft_strdup((*token)->content);
		if (!cmd->redirects[cmd->redirect_count].file)
			return (0);
		cmd->redirect_count++;
		return (1);
	}
	return (0);
}

t_command *parse_command_from_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*cur_cmd;
	t_command	*new_cmd;
	int			i;

	new_cmd = NULL;
	head = init_cmd();
	if (!head)
		return (NULL);
	cur_cmd = head;
	i = 0;
	while (tokens && i < MAX_ARGS)
	{
		if (tokens->type == TOKEN_WORD)
		{
			cur_cmd->argv[i] = ft_strdup(tokens->content);
			if (!cur_cmd->argv[i])
				return (NULL);
			i++;
		}
		else if (is_redirect_token(tokens->type))
		{
			if (cur_cmd->redirect_count >= MAX_REDIRECTS)
				return (NULL); //implement error handling
			if (!handle_redirection_parsing(cur_cmd, &tokens))
				return (NULL); // handle error
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			cur_cmd->argv[i] = NULL;
			cur_cmd->pipe_out = 1; // this node's output will be piped to the next node
			new_cmd = init_cmd();
			if (!new_cmd)
				return (NULL); //handle error
			cur_cmd->next = new_cmd;
			cur_cmd = new_cmd;
			i = 0;
		}
		tokens = tokens->next;
	}
	cur_cmd->argv[i] = NULL;
	cur_cmd = head;
	while (cur_cmd)
	{
		cur_cmd->path = ft_strdup(cur_cmd->argv[0]);
		if (!cur_cmd->path)
			return (NULL);
		cur_cmd = cur_cmd->next;
	}
	return (head);
}

void	print_cmd_list(t_command *head)
{
	t_command	*temp;

	printf("inside print cmd_lst\n");
	while (head)
	{
		temp = head;
		head = head->next;
		printf("command path-> '%s' redir count:%i\n", temp->path, temp->redirect_count);
        while (temp->argv && *temp->argv)
        {
            printf("arg: %s\n", *temp->argv);
            temp->argv++;
        }
	}
	printf("\n");
}