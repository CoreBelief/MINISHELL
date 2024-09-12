/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/21 11:00:43 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/12 17:43:41 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_file(char *target, t_token_type type)
{
	int	fd;

	fd = -1;
	if (type == TOKEN_REDIRECT_IN)
		fd = open(target, O_RDONLY);
	else if (type == TOKEN_REDIRECT_OUT)
		fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == TOKEN_REDIRECT_APPEND)
		fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 420);
	return (fd);
}

void	handle_file_opening(int *fd, char *target, t_token_type type)
{
	if (*fd != -1)
		close (*fd);
	*fd = open_file(target, type);
	if (*fd == -1)
	{
		perror("Error opening file for redirection");
		exit(EXIT_FAILURE);
	}

}

void	redirect_stream(int fd, t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("Error redirecting stdin");
			close(fd);
			exit(EXIT_FAILURE);
		}
		close (fd);
	}
	else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("Error redirecting stdout");
			close(fd);
			exit(EXIT_FAILURE);
		}
		close (fd);
	}
}

void	setup_redirections(t_command *cmd)
{
	int	cur;

	cur = 0;
	while (cur < cmd->redirect_count)
	{
		if (cmd->redir[cur].type == TOKEN_REDIRECT_IN)
			handle_file_opening(&cmd->input, cmd->redir[cur].file, \
			TOKEN_REDIRECT_IN);
		else
			if (cmd->redir[cur].type == TOKEN_REDIRECT_OUT || \
			cmd->redir[cur].type == TOKEN_REDIRECT_APPEND)
				handle_file_opening(&cmd->output, cmd->redir[cur].file, \
				cmd->redir[cur].type);
		cur++;
	}
	if (cmd->redir[cur - 1].type == TOKEN_REDIRECT_IN)
		redirect_stream(cmd->input, cmd->redir[cur - 1].type);
	else if (cmd->redir[cur - 1].type == TOKEN_REDIRECT_OUT || \
	cmd->redir[cur - 1].type == TOKEN_REDIRECT_APPEND)
		redirect_stream(cmd->output, cmd->redir[cur - 1].type);
	// if (cmd->redir[cur]->type == TOKEN_HEREDOC)
	// 	handle_heredoc(cmd);
}
