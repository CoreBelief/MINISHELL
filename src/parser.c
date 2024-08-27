/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/26 14:02:24 by eeklund       #+#    #+#                 */
/*   Updated: 2024/08/27 15:39:32 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
	cmd->redir = malloc (sizeof(t_redirect) * MAX_REDIRECTS);
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

static int	handle_token(t_token **tokens, t_command **cur_cmd, int *i)
{
	if (!is_redirect_token((*tokens)->type) && (*tokens)->type != TOKEN_PIPE)
		return (handle_arg_parsing_2nd(*cur_cmd, tokens, i));
	else if (is_redirect_token((*tokens)->type))
	{
		if ((*cur_cmd)->redirect_count >= MAX_REDIRECTS)
			return (0); //implement error handling
		if (!handle_redirection_parsing(*cur_cmd, tokens))
			return (0);
		// if ((*tokens)->next)
		// 	(*tokens) = (*tokens)->next;
	}
	else if ((*tokens)->type == TOKEN_PIPE)
	{
		*cur_cmd = handle_pipe_parsing(*cur_cmd, i);
		if (!*cur_cmd)
			return (0);
	}
	return (1);
}

// doesnt parse pipes correctly
t_command	*parse_command_from_tokens(t_token *tokens)
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
		if (!handle_token(&tokens, &cur_cmd, &i))
			return (NULL);
		tokens = tokens->next;
	}
	cur_cmd->argv[i] = NULL;
	set_command_paths(head);
	return (head);
}


// //VERY LONG BUT WORKS
// t_command *parse_command_from_tokens(t_token *tokens)
// {
// 	t_command	*head;
// 	t_command	*cur_cmd;
// 	t_command	*new_cmd;
// 	int			i;

// 	new_cmd = NULL;
// 	head = init_cmd();
// 	if (!head)
// 		return (NULL);
// 	cur_cmd = head;
// 	i = 0;
// 	// printf("inside parse_command_from_tokens\n");
// 	while (tokens && i < MAX_ARGS)
// 	{
// 	// printf("inside parse_command_from_tokens loop\n");

// 		if (!is_redirect_token(tokens->type) && tokens->type != TOKEN_PIPE)
// 		{
// 			if (tokens->type == TOKEN_DOUBLE_QUOTE)
// 				variable_exp_double(tokens, tokens->content);
// 			else if (tokens->content[0] == '$')
// 				variable_exp_dollar(tokens, tokens->content);
// 			cur_cmd->argv[i] = ft_strdup(tokens->content);
// 			if (!cur_cmd->argv[i])
// 				return (NULL);
// 			i++;
// 	// printf("inside parse_command_from_tokens loop inner\n");

// 		}
// 		else if (is_redirect_token(tokens->type))
// 		{
// 			if (cur_cmd->redirect_count >= MAX_REDIRECTS)
// 				return (NULL); //implement error handling
// 			if (!handle_redirection_parsing(cur_cmd, &tokens))
// 				return (NULL); // handle error
// 	// printf("inside parse_command_from_tokens if\n");

// 		}
// 		else if (tokens->type == TOKEN_PIPE)
// 		{
// 			cur_cmd->argv[i] = NULL;
// 			cur_cmd->pipe_out = 1; // this node's output will be piped to the next node
// 			new_cmd = init_cmd();
// 			if (!new_cmd)
// 				return (NULL); //handle error
// 			cur_cmd->next = new_cmd;
// 			cur_cmd = new_cmd;
// 			i = 0;
// 		}
// 		tokens = tokens->next;
// 	// printf("inside parse_command_from_tokens elif\n");
// 	}
// 	cur_cmd->argv[i] = NULL;
// 	cur_cmd = head;
// 	while (cur_cmd)
// 	{
// 		cur_cmd->path = ft_strdup(cur_cmd->argv[0]);
// 		if (!cur_cmd->path)
// 			return (NULL);
// 		cur_cmd = cur_cmd->next;
// 	// printf("inside parse_command_from_tokens last loop\n");

// 	}
// 	return (head);
// }