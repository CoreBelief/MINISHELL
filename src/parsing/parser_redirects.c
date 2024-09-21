/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_redirects.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/17 14:52:57 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/21 17:44:14 by eeklund       ########   odam.nl         */
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

int	handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell)
{
	char	*delim;
	int		hered_fd;
	// char 	*line;
	char 	*tmp_file;

	cmd->redir[cmd->redirect_count].type = (*token)->type;
	(*token) = (*token)->next;
	if (token && *token && (*token)->type == TOKEN_WORD)
	{
		delim = (*token)->content;
		tmp_file = ft_strjoin("/tmp/heredoc_", ft_itoa(cmd->redirect_count));
		hered_fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (hered_fd == -1)
		{
			perror("minishell: open");
			free(tmp_file);
			exit(EXIT_FAILURE);
		}
		while (1)
		{
			(*token)->content = readline("> ");
			if (!(*token)->content || ft_strcmp((*token)->content, delim) == 0)
				break ;
			variable_exp_double(*token, (*token)->content, shell);
			write(hered_fd, (*token)->content, strlen((*token)->content));
			write(hered_fd, "\n", 1);
			free((*token)->content);
		}
		if ((*token)->content)
			free((*token)->content);
		close(hered_fd);
		cmd->redir[cmd->redirect_count].file = tmp_file;
		if (!cmd->redir[cmd->redirect_count].file)
			return (0);
		cmd->redirect_count++;
		return (1);
	}
	return (0);
}

int	handle_redirection_parsing(t_cmd *cmd, t_token **token)
{
	cmd->redir[cmd->redirect_count].type = (*token)->type;
	(*token) = (*token)->next;
	if (token && *token && ((*token)->type == TOKEN_WORD || (*token)->type == TOKEN_DOUBLE_QUOTE || (*token)->type == TOKEN_SINGLE_QUOTE))
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