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
	(*token) = (*token)->next;
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

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	find_var_len(char *var)
{
	int	i;

	i = 0;
	while (var[i] && is_var_char(var[i]))
		i++;
	return (i);
}

void	variable_exp_dollar(t_token *token, char *str)
{
	int		i;
	int		len;
	char	*expansion;
	char	*new_str;
	char	*var;

	i = 0;
	expansion = NULL;
	new_str = ft_strdup("");
	//error handling
	if (str[i] == '$' && str[i + 1])
	{
		i++;
		len = find_var_len(&str[i]);
		var = ft_strndup(&str[i], len);
		//error handling
		expansion = getenv(var);
		free (var);
		if (!expansion)
		{
			printf("expansion variable not found\n");
			free (token->content);
			token->content = "";
			return ;
		}
		new_str = ft_strjoin(new_str, expansion);
		if (!new_str)
			return ;
		i += len;
		len = ft_strlen(&str[i]);
		new_str = ft_strjoin(new_str, &str[i]);
		if (!new_str)
			return ;
		token->content = new_str;
		free (str);
	}
}

int	until_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	return (i);
}

void	variable_exp_double(t_token *token, char *str)
{
	int		i;
	int		len;
	char	*expansion;
	char	*new_str;
	char	*var;

	i = 0;
	expansion = NULL;
	new_str = ft_strdup("");
	//error handling
	while (str[i])
	{
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
		{
			i++;
			len = find_var_len(&str[i]);
			var = ft_strndup(&str[i], len);
			//error handling
			expansion = getenv(var);
			free (var);
			if (!expansion)
			{
				printf("expansion variable not found\n");
				free (token->content);
				token->content = "";
				return ;
			}
			new_str = ft_strjoin(new_str, expansion);
			if (!new_str)
				return ;
			i += len;
			len = until_dollar(&str[i]);
			char *middle = ft_strndup(&str[i], len);
			new_str = ft_strjoin(new_str, middle);
			if (!new_str)
				return ;
			free (middle);
			i += len;
			token->content = new_str;
		}
		else
			i++;
	}
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
	// printf("inside parse_command_from_tokens\n");
	while (tokens && i < MAX_ARGS)
	{
	// printf("inside parse_command_from_tokens loop\n");

		if (!is_redirect_token(tokens->type) && tokens->type != TOKEN_PIPE)
		{
			if (tokens->type == TOKEN_DOUBLE_QUOTE)
				variable_exp_double(tokens, tokens->content);
			else if (tokens->content[0] == '$')
				variable_exp_dollar(tokens, tokens->content);
			cur_cmd->argv[i] = ft_strdup(tokens->content);
			if (!cur_cmd->argv[i])
				return (NULL);
			i++;
	// printf("inside parse_command_from_tokens loop inner\n");

		}
		else if (is_redirect_token(tokens->type))
		{
			if (cur_cmd->redirect_count >= MAX_REDIRECTS)
				return (NULL); //implement error handling
			if (!handle_redirection_parsing(cur_cmd, &tokens))
				return (NULL); // handle error
	// printf("inside parse_command_from_tokens if\n");

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
	// printf("inside parse_command_from_tokens elif\n");
	}
	cur_cmd->argv[i] = NULL;
	cur_cmd = head;
	while (cur_cmd)
	{
		cur_cmd->path = ft_strdup(cur_cmd->argv[0]);
		if (!cur_cmd->path)
			return (NULL);
		cur_cmd = cur_cmd->next;
	// printf("inside parse_command_from_tokens last loop\n");

	}
	return (head);
}

void	print_cmd_list(t_command *head)
{
	// t_command	*temp;
	int			i;
	int			j;

	printf("inside print cmd_lst\n");
	// temp = head;
	while (head)
	{
		printf("command path-> '%s' redir count:%i\n", head->path, head->redirect_count);
		i = 0;
		while (head->argv && head->argv[i])
		{
			printf("arg: %s\n", head->argv[i]);
			i++;
		}
		if (head->redirect_count != 0)
		{
			printf("redirecttype == %i target file == %s\n", head->redirects->type, head->redirects->file);
		}
	    printf("  Pipe In: %d, Pipe Out: %d\n", head->pipe_in, head->pipe_out);
		j = 0;
        for (j = 0; j < head->redirect_count; j++)
        {
            printf("  Redirect[%d] Type: %d, File: %s\n", j, head->redirects[j].type, head->redirects[j].file);
        }
		head = head->next;
	}
	printf("\n");
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