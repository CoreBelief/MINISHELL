/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_parsing.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 15:46:18 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 14:26:17 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L

#include "minishell.h"
#include "signal.h"
#define ERR_SYNTAX 2
#define ERR_MALLOC 1
#define ERR_FILE_OPEN 1
#define ERR_FORK 1

static int	handle_heredoc_child(char *delim, t_shell *shell, int hered_fd);
int			handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell);
static void	handle_signals(struct sigaction *old_int,
				struct sigaction *old_quit, int restore);
static int	handle_heredoc_parent(pid_t pid, int hered_fd,
				struct sigaction *old_int, struct sigaction *old_quit);
static int	finalize_heredoc(t_cmd *cmd, char *tmp_file, int status);

static void	handle_signals(struct sigaction *old_int,
		struct sigaction *old_quit, int restore)
{
	if (restore)
	{
		sigaction(SIGINT, old_int, NULL);
		sigaction(SIGQUIT, old_quit, NULL);
	}
	else
	{
		setup_signal(SIGINT, SIG_IGN, 0);
		setup_signal(SIGQUIT, SIG_IGN, 0);
		sigaction(SIGINT, NULL, old_int);
		sigaction(SIGQUIT, NULL, old_quit);
	}
}

static int	handle_heredoc_child(char *delim, t_shell *shell, int hered_fd)
{
	setup_signal(SIGINT, SIG_DFL, 0);
	setup_signal(SIGQUIT, SIG_IGN, 0);
	if (!write_to_hdfd(delim, shell, hered_fd))
		return (handle_syn_errors(1, "Malloc fail\n", shell), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	handle_heredoc_parent(pid_t pid, int hered_fd,
		struct sigaction *old_int, struct sigaction *old_quit)
{
	int	status;

	waitpid(pid, &status, 0);
	close(hered_fd);
	handle_signals(old_int, old_quit, 1);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

static int	finalize_heredoc(t_cmd *cmd, char *tmp_file, int status)
{
	if (status == EXIT_SUCCESS)
	{
		cmd->redir[cmd->redirect_count].file = tmp_file;
		if (!cmd->redir[cmd->redirect_count].file)
		{
			free(tmp_file);
			return (0);
		}
		cmd->redirect_count++;
		return (1);
	}
	free(tmp_file);
	return (0);
}

int	handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell)
{
	int					hered_fd;
	char				*tmp_file;
	pid_t				pid;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	if (!validate_heredoc_syntax(cmd, token, shell))
		return (0);
	tmp_file = create_and_open_temp_file(cmd->redirect_count, &hered_fd);
	if (!tmp_file)
		return (handle_syn_errors(1, "Error opening file\n", shell), 0);
	handle_signals(&old_int, &old_quit, 0);
	pid = fork();
	if (pid == -1)
	{
		handle_signals(&old_int, &old_quit, 1);
		close(hered_fd);
		free(tmp_file);
		return (handle_syn_errors(1, "Could not fork:(\n", shell), 0);
	}
	if (pid == 0)
		exit(handle_heredoc_child((*token)->content, shell, hered_fd));
	return (finalize_heredoc(cmd, tmp_file,
			handle_heredoc_parent(pid, hered_fd, &old_int, &old_quit)));
}
