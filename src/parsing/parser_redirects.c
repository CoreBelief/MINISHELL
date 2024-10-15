/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_redirects.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/17 14:52:57 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 15:33:28 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect_token(int token_type);
int	handle_redirection_parsing(t_cmd *cmd, t_token **token, t_shell *shell);

int	is_redirect_token(int token_type)
{
	return (token_type == TOKEN_REDIRECT_IN || \
		token_type == TOKEN_REDIRECT_OUT || \
		token_type == TOKEN_REDIRECT_APPEND || \
		token_type == TOKEN_HEREDOC);
}

int	handle_redirection_parsing(t_cmd *cmd, t_token **token, t_shell *shell)
{
	char	*redir_file;

	cmd->redir[cmd->redirect_count].type = (*token)->type;
	(*token) = (*token)->next;
	if (token && *token && ((*token)->type == TOKEN_WORD || \
	(*token)->type == TOKEN_DOUBLE_QUOTE || \
	(*token)->type == TOKEN_SINGLE_QUOTE))
	{
		redir_file = ft_strdup((*token)->content);
		if (!redir_file)
			return (handle_syn_errors(1, "Malloc fail\n", shell), 0);
		cmd->redir[cmd->redirect_count].file = redir_file;
		cmd->redirect_count++;
		return (1);
	}
	handle_syn_errors(2, "syntax error near redirection\n", shell);
	return (0);
}
