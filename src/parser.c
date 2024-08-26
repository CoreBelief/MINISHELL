#include "parser.h"

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

static int	handle_token(t_token *tokens, t_command **cur_cmd, int *i)
{
	if (!is_redirect_token(tokens->type) && tokens->type != TOKEN_PIPE)
		return (handle_arg_parsing(*cur_cmd, &tokens, i));
	else if (is_redirect_token(tokens->type))
	{
		if ((*cur_cmd)->redirect_count >= MAX_REDIRECTS)
			return (0); //implement error handling
		return (handle_redirection_parsing(*cur_cmd, &tokens));
	}
	else if (tokens->type == TOKEN_PIPE)
	{
		*cur_cmd = handle_pipe_parsing(*cur_cmd, i);
		if (!*cur_cmd)
			return (0);
	}
	return (1);
}

t_command *parse_command_from_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*cur_cmd;
	int			i;

	head = init_cmd();
	if (!head)
		return (NULL);
	cur_cmd = head;
	i = 0;
	while (tokens && i < MAX_ARGS)
	{
		if (!handle_token(tokens, &cur_cmd, &i))
			return (NULL);
		tokens = tokens->next;
	}
	cur_cmd->argv[i] = NULL;
	set_command_paths(head);
	return (head);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

// char *find_command_path(char *command)
// {
//     char *path = getenv("PATH");
//     char *path_copy = ft_strdup(path);
//     //illeagal strtok!!!
//     char *dir = strtok(path_copy, ":");
//     char *full_path = NULL;

//     while (dir != NULL) {
//         full_path = malloc(strlen(dir) + ft_strlen(command) + 2);
//         sprintf(full_path, "%s/%s", dir, command);
        
//         if (access(full_path, X_OK) == 0) {
//             free(path_copy);
//             return full_path;
//         }
        
//         free(full_path);
//         dir = strtok(NULL, ":");
//     }

//     free(path_copy);
//     return ft_strdup(command);  // Return the command as-is if not found in PATH
// }


// t_command *parse_command(char *input)
// {
//     t_command *cmd;
//     t_token *tokens;
//     int argc = 0;
//     int i;

//     cmd = malloc(sizeof(t_command));
//     if (!cmd)
//         return NULL;

//     // Initialize all fields to NULL or 0
//     cmd->path = NULL;
//     cmd->argv = NULL;
//     cmd->redirects = NULL;
//     cmd->redirect_count = 0;
//     cmd->pipe_in = STDIN_FILENO;
//     cmd->pipe_out = STDOUT_FILENO;

//     tokens = tokenizer(input);
//     if (!tokens)
//     {
//         free(cmd);
//         return NULL;
//     }
//     print_token_list(tokens);
//     // Count the number of word tokens to determine argc
//     for (t_token *t = tokens; t != NULL; t = t->next)
//     {
//         if (t->type == TOKEN_WORD)
//             argc++;
//     }

//     cmd->argv = malloc(sizeof(char *) * (argc + 1));
//     if (!cmd->argv)
//     {
//         free(cmd);
//         free_tokens(&tokens);
//         return NULL;
//     }

//     // Fill argv array
//     i = 0;
//     for (t_token *t = tokens; t != NULL; t = t->next)
//     {
//         if (t->type == TOKEN_WORD)
//         {
//             cmd->argv[i] = ft_strdup(t->content);
//             i++;
//         }
//         // Here you can handle redirections and other token types
//     }
//     cmd->argv[i] = NULL;

//     if (argc > 0)
//         cmd->path = find_command_path(cmd->argv[0]);

//     free_tokens(&tokens);
//     return cmd;
// }