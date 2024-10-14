/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/26 14:02:24 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 18:48:38 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd		*init_cmd(void);
static int	handle_token(t_token **tok, t_cmd **cur_cmd, \
int *i, t_shell *shell);
int			parse_command_from_tokens(t_shell *shell);

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
	{
		if (!handle_word_parsing(*cur_cmd, tok, i))
			return (handle_syn_errors(1, "Malloc fail\n", shell), 0);
	}
	else if (is_redirect_token((*tok)->type))
	{
		if ((*cur_cmd)->redirect_count >= MAX_REDIRECTS)
			return (handle_syn_errors(1, "Too many redirects\n", shell), 0);
		if ((*tok)->type == TOKEN_HEREDOC)
		{
			if (!(handle_heredoc_parsing(*cur_cmd, tok, shell)))
				return (0);
		}
		else if (!handle_redirection_parsing(*cur_cmd, tok, shell))
			return (0);
	}
	else if ((*tok)->type == TOKEN_PIPE)
	{
		*cur_cmd = handle_pipe_parsing(*cur_cmd, i);
		if (!*cur_cmd)
			return (handle_syn_errors(1, " Malloc fail\n", shell), 0);
	}
	return (1);
}

int	parse_command_from_tokens(t_shell *shell)
{
	t_cmd	*cur_cmd;
	t_token	*cur_tok;
	int		i;

	cur_tok = shell->tokens;
	shell->commands = init_cmd();
	if (!shell->commands)
		return (0);
	cur_cmd = shell->commands;
	i = 0;
	while (cur_tok && i < MAX_ARGS)
	{
		if (!handle_token(&cur_tok, &cur_cmd, &i, shell))
		{
			cur_cmd->argv[i] = NULL;
			return (free_command_list(&shell->commands), 0);
		}
		cur_tok = (cur_tok)->next;
	}
	cur_cmd->argv[i] = NULL;
	return (1);
}
