/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/26 14:02:24 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/04 15:46:26 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd		*init_cmd(void);
static int	handle_token(t_token **tok, t_cmd **cur_cmd, \
int *i, t_shell *shell);
int			parse_command_from_tokens(t_token *tokens, t_shell *shell);

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *) * (MAX_ARGS + 1));
	if (!cmd->argv)
		return (free(cmd), NULL);
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
	cmd->expansion = 0;
	cmd->next = NULL;
	return (cmd);
}

static int	handle_token(t_token **tok, t_cmd **cur_cmd, int *i, t_shell *shell)
{
	if (!is_redirect_token((*tok)->type) && (*tok)->type != TOKEN_PIPE)
		return (handle_word_parsing(*cur_cmd, tok, i)); //returns 0 on malloc fail
	else if (is_redirect_token((*tok)->type))
	{
		if ((*cur_cmd)->redirect_count >= MAX_REDIRECTS)
			return (0); //error: more redirects than ok
		if ((*tok)->type == TOKEN_HEREDOC)
		{
			if (!(handle_heredoc_parsing(*cur_cmd, tok, shell))) // have many different exit codes. implement exit codes and messages inside func and only return 0 here
				return (0);
		}
		else if (!handle_redirection_parsing(*cur_cmd, tok, shell))
			return (0); // can be syntax error or malloc fal so only return 0 here and handle exit codes/msgs inside
	}
	else if ((*tok)->type == TOKEN_PIPE)
	{
		*cur_cmd = handle_pipe_parsing(*cur_cmd, i);
		if (!*cur_cmd)
			return (0); // always malloc fail
	}
	return (1);
}

int	parse_command_from_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd	*cur_cmd;
	int		i;

	shell->commands = init_cmd();
	if (!shell->commands)
		return (0);
	cur_cmd = shell->commands;
	i = 0;
	while (tokens && i < MAX_ARGS)
	{
		if (!handle_token(&tokens, &cur_cmd, &i, shell)) // all error codes are set before coming here
		{
			cur_cmd->argv[i] = NULL;
			return (free_command_list(&shell->commands), 0);
		}
		tokens = tokens->next;
	}
	cur_cmd->argv[i] = NULL;
	return (1);
}
