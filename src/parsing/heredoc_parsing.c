/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_parsing.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 15:46:18 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/26 15:46:45 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*create_filename(int redir_count)
{
	char	*filename;
	char	*hd_id;

	hd_id = ft_itoa(redir_count);
	if (!hd_id)
		return (0);
	filename = ft_strjoin("/tmp/heredoc_", hd_id);
	free (hd_id);
	if (!filename)
		return (0);
	return (filename);
}

int	open_hdfile(char *target)
{
	int	hd_fd;

	hd_fd = open(target, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (hd_fd == -1)
	{
		perror("minishell: open");
		free(target);
		// free_all()
		exit(EXIT_FAILURE);
	}
	return (hd_fd);
}

int	write_to_hdfd(t_token *token, char *delim, t_shell *shell, int hdfd)
{
	char	*tmp;

	while (1)
	{
		token->content = readline("> ");
		if (!token->content || ft_strcmp(token->content, delim) == 0)
			break ;
		tmp = token->content;
		if (!variable_exp_double(token, tmp, shell))
		{
			free(tmp);
			free(delim);
			return (0);
		}
		write(hdfd, token->content, strlen(token->content));
		write(hdfd, "\n", 1);
		free (tmp);
		free(token->content);
	}
	free (delim);
	close(hdfd);
	return (1);
}

int	handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell)
{
	char	*delim;
	int		hered_fd;
	char	*tmp_file;

	cmd->redir[cmd->redirect_count].type = (*token)->type;
	(*token) = (*token)->next;
	if (token && *token && (*token)->type == TOKEN_WORD)
	{
		delim = (*token)->content;
		tmp_file = create_filename(cmd->redirect_count);
		hered_fd = open_hdfile(tmp_file);
		if (!write_to_hdfd(*token, delim, shell, hered_fd))
			return (0);
		cmd->redir[cmd->redirect_count].file = tmp_file;
		if (!cmd->redir[cmd->redirect_count].file)
			return (0);
		cmd->redirect_count++;
		return (1);
	}
	return (0);
}