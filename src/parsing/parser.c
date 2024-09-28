/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/26 14:02:24 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/28 19:20:49 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
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
	return (cmd);
}

static int	handle_token(t_token **tok, t_cmd **cur_cmd, int *i, t_shell *shell)
{
	if (!is_redirect_token((*tok)->type) && (*tok)->type != TOKEN_PIPE)
		return (handle_word_parsing(*cur_cmd, tok, i));
	else if (is_redirect_token((*tok)->type))
	{
		if ((*cur_cmd)->redirect_count >= MAX_REDIRECTS)
			return (0);
		if ((*tok)->type == TOKEN_HEREDOC)
		{
			if (!(handle_heredoc_parsing(*cur_cmd, tok, shell)))
				return (0);
		}
		else if (!handle_redirection_parsing(*cur_cmd, tok))
			return (0);
		// if ((*tok)->next)
		// 	(*tok) = (*tok)->next;
	}
	else if ((*tok)->type == TOKEN_PIPE)
	{
		// printf("%i index", *i);
		*cur_cmd = handle_pipe_parsing(*cur_cmd, i);
		if (!*cur_cmd)
			return (0);
		// printf("cur_cmd in handle_token %s\n", (*cur_cmd)->argv[0]);
	}
	return (1);
}

// DOESNT PARSE PIPES CORRECTLY
t_cmd	*parse_command_from_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*cur_cmd;
	int		i;

	head = init_cmd();
	if (!head)
		return (NULL);
	cur_cmd = head;
	i = 0;
	while (tokens && i < MAX_ARGS)
	{
		if (!handle_token(&tokens, &cur_cmd, &i, shell))
			return (NULL);
		tokens = tokens->next;
	}
	cur_cmd->argv[i] = NULL;
	// printf("token cur next\n");
	// set_cmd_paths(head); // im not sure we are ever using the path so might be unnecessary
	return (head);
}
