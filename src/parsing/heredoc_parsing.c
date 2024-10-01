/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_parsing.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 15:46:18 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/01 21:36:15 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signal.h"

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
		return (-1);
	}
	return (hd_fd);
}

int	write_to_hdfd(char *delim, t_shell *shell, int hdfd)
{
	char	*content;
	char	*expansion;

	while (1)
	{	
		content = readline("> ");
		if (!content)
			break ;
		if (ft_strcmp(content, delim) == 0)
		{
			free (content);
			break ;
		}
		expansion = variable_exp_double(content, shell);
		free(content);
		if (!expansion)
		{
			return (0);
		}
		write(hdfd, expansion, ft_strlen(expansion));
		write(hdfd, "\n", 1);
		free(expansion);
	}
	close(hdfd);
	return (1);
}

// static void	ignore_signals(struct sigaction *old_int, struct sigaction *old_quit)
// {
// 	struct sigaction	sa;

// 	sa.sa_handler = SIG_IGN;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sigaction(SIGINT, &sa, old_int);
// 	sigaction(SIGQUIT, &sa, old_quit);
// }

// static void	restore_signals(struct sigaction *old_int, struct sigaction *old_quit)
// {
// 	sigaction(SIGINT, old_int, NULL);
// 	sigaction(SIGQUIT, old_quit, NULL);
// }

// static int	handle_heredoc_child(char *delim, t_shell *shell, int hered_fd)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	if (!write_to_hdfd(delim, shell, hered_fd))
// 		return (EXIT_FAILURE);
// 	return (EXIT_SUCCESS);
// }

// int	handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell)
// {
// 	char				*delim;
// 	int					hered_fd;
// 	char				*tmp_file;
// 	pid_t				pid;
// 	int					status;
// 	struct sigaction	old_int;
// 	struct sigaction	old_quit;

// 	cmd->redir[cmd->redirect_count].type = (*token)->type;
// 	*token = (*token)->next;
// 	if (!(*token) || (*token)->type != TOKEN_WORD)
// 		return (0);
// 	delim = (*token)->content;
// 	tmp_file = create_filename(cmd->redirect_count);
// 	hered_fd = open_hdfile(tmp_file);
// 	if (hered_fd == -1)
// 		return (free(tmp_file), 0);
// 	ignore_signals(&old_int, &old_quit);
// 	pid = fork();
// 	if (pid == -1)
// 		return (restore_signals(&old_int, &old_quit), close(hered_fd),
// 			free(tmp_file), 0);
// 	if (pid == 0)
// 		exit(handle_heredoc_child(delim, shell, hered_fd));
// 	waitpid(pid, &status, 0);
// 	close(hered_fd);
// 	restore_signals(&old_int, &old_quit);
// 	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
// 	{
// 		cmd->redir[cmd->redirect_count].file = tmp_file;
// 		if (!cmd->redir[cmd->redirect_count].file)
// 			return (free(tmp_file), 0);
// 		cmd->redirect_count++;
// 		return (1);
// 	}
// 	return (free(tmp_file), 0);
// }

// static void	ignore_signals(struct sigaction *old_int, struct sigaction *old_quit)
// {
// 	setup_signal(SIGINT, SIG_IGN, 0);
// 	setup_signal(SIGQUIT, SIG_IGN, 0);
// 	sigaction(SIGINT, NULL, old_int);
// 	sigaction(SIGQUIT, NULL, old_quit);
// }

// static void	restore_signals(struct sigaction *old_int, struct sigaction *old_quit)
// {
// 	sigaction(SIGINT, old_int, NULL);
// 	sigaction(SIGQUIT, old_quit, NULL);
// }

// static int	handle_heredoc_child(char *delim, t_shell *shell, int hered_fd)
// {
// 	setup_signals_child();
// 	if (!write_to_hdfd(delim, shell, hered_fd))
// 		return (EXIT_FAILURE);
// 	return (EXIT_SUCCESS);
// }

// int	handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell)
// {
// 	char				*delim;
// 	int					hered_fd;
// 	char				*tmp_file;
// 	pid_t				pid;
// 	int					status;
// 	struct sigaction	old_int;
// 	struct sigaction	old_quit;

// 	cmd->redir[cmd->redirect_count].type = (*token)->type;
// 	*token = (*token)->next;
// 	if (!(*token) || (*token)->type != TOKEN_WORD)
// 		return (0);
// 	delim = (*token)->content;
// 	tmp_file = create_filename(cmd->redirect_count);
// 	hered_fd = open_hdfile(tmp_file);
// 	if (hered_fd == -1)
// 		return (free(tmp_file), 0);
// 	ignore_signals(&old_int, &old_quit);
// 	pid = fork();
// 	if (pid == -1)
// 		return (restore_signals(&old_int, &old_quit), close(hered_fd),
// 			free(tmp_file), 0);
// 	if (pid == 0)
// 		exit(handle_heredoc_child(delim, shell, hered_fd));
// 	waitpid(pid, &status, 0);
// 	close(hered_fd);
// 	restore_signals(&old_int, &old_quit);
// 	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
// 	{
// 		cmd->redir[cmd->redirect_count].file = tmp_file;
// 		if (!cmd->redir[cmd->redirect_count].file)
// 			return (free(tmp_file), 0);
// 		cmd->redirect_count++;
// 		return (1);
// 	}
// 	return (free(tmp_file), 0);
// }

static void	ignore_signals(struct sigaction *old_int, struct sigaction *old_quit)
{
	setup_signal(SIGINT, SIG_IGN, 0);
	setup_signal(SIGQUIT, SIG_IGN, 0);
	sigaction(SIGINT, NULL, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
}

static void	restore_signals(struct sigaction *old_int, struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}

static int	handle_heredoc_child(char *delim, t_shell *shell, int hered_fd)
{
	setup_signal(SIGINT, SIG_DFL, 0);
	setup_signal(SIGQUIT, SIG_IGN, 0);
	if (!write_to_hdfd(delim, shell, hered_fd))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell)
{
	char				*delim;
	int					hered_fd;
	char				*tmp_file;
	pid_t				pid;
	int					status;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	cmd->redir[cmd->redirect_count].type = (*token)->type;
	*token = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN_WORD)
		return (0);
	delim = (*token)->content;
	tmp_file = create_filename(cmd->redirect_count);
	hered_fd = open_hdfile(tmp_file);
	if (hered_fd == -1)
		return (free(tmp_file), 0);
	ignore_signals(&old_int, &old_quit);
	pid = fork();
	if (pid == -1)
		return (restore_signals(&old_int, &old_quit), close(hered_fd),
			free(tmp_file), 0);
	if (pid == 0)
		exit(handle_heredoc_child(delim, shell, hered_fd));
	waitpid(pid, &status, 0);
	close(hered_fd);
	restore_signals(&old_int, &old_quit);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
	{
		cmd->redir[cmd->redirect_count].file = tmp_file;
		if (!cmd->redir[cmd->redirect_count].file)
			return (free(tmp_file), 0);
		cmd->redirect_count++;
		return (1);
	}
	return (free(tmp_file), 0);
}