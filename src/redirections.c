/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/21 11:00:43 by eeklund       #+#    #+#                 */
/*   Updated: 2024/08/27 12:55:41 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_redirections(t_command *cmd)
{
	int	fd;
	int	cur;

	cur = 0;
	while (cur < cmd->redirect_count)
	{
		if (cmd->redir[cur].type == TOKEN_REDIRECT_OUT)
			fd = open(cmd->redir[cur].file, O_WRONLY | O_CREAT | O_TRUNC, 420);
		else if (cmd->redir[cur].type == TOKEN_REDIRECT_APPEND)
			fd = open(cmd->redir[cur].file, O_WRONLY | O_CREAT | O_APPEND, 420);
		else if (cmd->redir[cur].type == TOKEN_REDIRECT_IN)
			fd = open(cmd->redir[cur].file, O_RDONLY);
		if (fd == -1)
		{
			perror("Error opening file for redirection");
			return ;
		}
		if ((cur == cmd->redirect_count - 1)  && (cmd->redir[cur].type == \
		TOKEN_REDIRECT_OUT || cmd->redir[cur].type == TOKEN_REDIRECT_APPEND))
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("Error redirecting stdout");
				close(fd);
				return ;
			}
		}
		else if (cmd->redir[cur].type == TOKEN_REDIRECT_IN)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("Error redirecting stdout");
				close(fd);
				return ;
			}
		}
		close(fd);
		cur++;
	}

	// if (cmd->redirects->type == TOKEN_HEREDOC)
	// {
	// 	handle_heredoc(cmd);
	// }

	// printf("DEBUG: setup_redirections called\n");
}
