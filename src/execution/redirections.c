/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/21 11:00:43 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/05 16:05:01 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_file(char *target, t_token_type type)
{
	int	fd;

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

// NEEDS TO BE FIXED
// flow of input/output, what should be redirected, also maybe not use dup here but wait til later
// redircetions in combination with pipes is still fucked
// void	setup_redirections(t_command *cmd)
// {
// 	// int	fd;
// 	int	cur;

// 	cur = 0;
// 	while (cur < cmd->redirect_count)
// 	{
// 		if (cmd->redir[cur].type == TOKEN_REDIRECT_OUT)
// 			cmd->output = open(cmd->redir[cur].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		else if (cmd->redir[cur].type == TOKEN_REDIRECT_APPEND)
// 			cmd->output = open(cmd->redir[cur].file, O_WRONLY | O_CREAT | O_APPEND, 420);
// 		else if (cmd->redir[cur].type == TOKEN_REDIRECT_IN)
// 			cmd->output = open(cmd->redir[cur].file, O_RDONLY); // unclear if it shoul be input or output
// 		if (cmd->output == -1)
// 		{
// 			perror("Error opening file for redirection");
// 			return ;
// 		}
// 		cur++;
// 	}
// 	cur--;
// 	if ((cmd->redir[cur].type == TOKEN_REDIRECT_OUT || 
// 	cmd->redir[cur].type == TOKEN_REDIRECT_APPEND))
// 	{
// 		if (dup2(cmd->output, STDOUT_FILENO) == -1)
// 		{
// 			perror("Error redirecting stdout");
// 			close(cmd->output);
// 			return ;
// 		}
// 	}
// 	else if (cmd->redir[cur].type == TOKEN_REDIRECT_IN)
// 	{
// 		if (dup2(cmd->output, STDIN_FILENO) == -1)
// 		{
// 			perror("Error redirecting stdout");
// 			close(cmd->output);
// 			return ;
// 		}
// 	}
// 	close(cmd->output);


	// if (cmd->redirects->type == TOKEN_HEREDOC)
	// {
	// 	handle_heredoc(cmd);
	// }

	// printf("DEBUG: setup_redirections called\n");
// }


// if ((cur == cmd->redirect_count - 1)  && (cmd->redir[cur].type == 
// TOKEN_REDIRECT_OUT || cmd->redir[cur].type == TOKEN_REDIRECT_APPEND))
// {
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		perror("Error redirecting stdout");
// 		close(fd);
// 		return ;
// 	}
// }
// else if (cmd->redir[cur].type == TOKEN_REDIRECT_IN)
// {
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{
// 		perror("Error redirecting stdout");
// 		close(fd);
// 		return ;
// 	}
// }