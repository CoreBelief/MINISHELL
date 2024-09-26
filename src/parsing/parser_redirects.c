/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_redirects.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/17 14:52:57 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/26 15:51:26 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

int	is_redirect_token(int token_type)
{
	return (token_type == TOKEN_REDIRECT_IN || \
		token_type == TOKEN_REDIRECT_OUT || \
		token_type == TOKEN_REDIRECT_APPEND || \
		token_type == TOKEN_HEREDOC);
}

int	handle_redirection_parsing(t_cmd *cmd, t_token **token)
{
	cmd->redir[cmd->redirect_count].type = (*token)->type;
	(*token) = (*token)->next;
	if (token && *token && ((*token)->type == TOKEN_WORD || \
	(*token)->type == TOKEN_DOUBLE_QUOTE || \
	(*token)->type == TOKEN_SINGLE_QUOTE))
	{
		cmd->redir[cmd->redirect_count].file = ft_strdup((*token)->content);
		if (!cmd->redir[cmd->redirect_count].file)
			return (0);
		cmd->redirect_count++;
		return (1);
	}
	printf("syntax error near redirection\n");
	return (0);
}
