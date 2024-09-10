/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/26 14:02:24 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/11 00:27:15 by rdl           ########   odam.nl         */
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
	cmd->pipe_in = -1;
	cmd->pipe_out = -1;
	cmd->input = -1;
	cmd->output = -1;
	cmd->next = NULL;
	cmd->path = NULL;
	return (cmd);
}

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

// DOESNT PARSE PIPES CORRECTLY
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
