/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_parsing.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 15:46:18 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 15:57:35 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L

#include "minishell.h"
#include "signal.h"
#define ERR_SYNTAX 2
#define ERR_MALLOC 1
#define ERR_FILE_OPEN 1
#define ERR_FORK 1

static char	*create_filename(int redir_count);
static int	open_hdfile(char *target);
static int	write_to_hdfd(char *delim, t_shell *shell, int hdfd);
static int	handle_heredoc_child(char *delim, t_shell *shell, int hered_fd);
int			handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell);

static int	validate_heredoc_syntax(t_cmd *cmd, t_token **token,
				t_shell *shell);
static char	*create_and_open_temp_file(int redirect_count, int *hered_fd);
static void	handle_signals(struct sigaction *old_int,
				struct sigaction *old_quit, int restore);
static int	handle_heredoc_parent(pid_t pid, int hered_fd,
				struct sigaction *old_int, struct sigaction *old_quit);
static int	finalize_heredoc(t_cmd *cmd, char *tmp_file, int status);

static char	*create_filename(int redir_count)
{
	char	*filename;
	char	*hd_id;

	hd_id = ft_itoa(redir_count);
	if (!hd_id)
		return (0);
	filename = ft_strjoin("/tmp/heredoc_", hd_id);
	free(hd_id);
	if (!filename)
		return (0);
	return (filename);
}

static int	open_hdfile(char *target)
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

static int	write_to_hdfd(char *delim, t_shell *shell, int hdfd)
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
			free(content);
			break ;
		}
		expansion = variable_exp_double(content, shell);
		free(content);
		if (!expansion)
		{
			return (0);
		}
		write(hdfd, expansion, ft_strlen(expansion)); //write can fail?
		write(hdfd, "\n", 1);
		free(expansion);
	}
	close(hdfd);
	return (1);
}

static int	handle_heredoc_child(char *delim, t_shell *shell, int hered_fd)
{
	setup_signal(SIGINT, SIG_DFL, 0);
	setup_signal(SIGQUIT, SIG_IGN, 0);
	if (!write_to_hdfd(delim, shell, hered_fd)) // malloc fail if return 0
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell)
{
	char	*delim;
	int		hered_fd;
	char	*tmp_file;
	pid_t	pid;
	int		status;

	struct sigaction old_int, old_quit;
	if (!validate_heredoc_syntax(cmd, token, shell))
		return (0);
	delim = (*token)->content;
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
		exit(handle_heredoc_child(delim, shell, hered_fd)); // exit w exit code 1, malloc fail, or 0, all goed
	status = handle_heredoc_parent(pid, hered_fd, &old_int, &old_quit);
	return (finalize_heredoc(cmd, tmp_file, status)); //signal error? 
}

static int	validate_heredoc_syntax(t_cmd *cmd, t_token **token, t_shell *shell)
{
	cmd->redir[cmd->redirect_count].type = (*token)->type;
	(*token) = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN_WORD)
	{
		shell->last_exit_status = ERR_SYNTAX;
		handle_syn_errors(ERR_SYNTAX, "syntax error near redirection\n", shell);
		return (0);
	}
	return (1);
}

static char	*create_and_open_temp_file(int redirect_count, int *hered_fd)
{
	char	*tmp_file;

	tmp_file = create_filename(redirect_count);
	if (!tmp_file)
		return (NULL);
	*hered_fd = open_hdfile(tmp_file);
	if (*hered_fd == -1)
	{
		free(tmp_file);
		return (NULL);
	}
	return (tmp_file);
}

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
		if (!cmd->redir[cmd->redirect_count].file) // is this check needed? 
		{
			free(tmp_file);
			return (0);
		}
		cmd->redirect_count++;
		return (1);
	}
	free(tmp_file);
	return (0); // signal error??
}
