/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_parsing_utils.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 18:26:23 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 18:50:06 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_filename(int redir_count);
static int	open_hdfile(char *target);
int			write_to_hdfd(char *delim, t_shell *shell, int hdfd);
int			validate_heredoc_syntax(t_cmd *cmd, t_token **token,
				t_shell *shell);
char		*create_and_open_temp_file(int redirect_count, int *hered_fd);

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
int write_to_hdfd(char *delim, t_shell *shell, int hdfd)
{
    char *content;
    char *expansion;

    while (1)
    {
        if (isatty(STDIN_FILENO)) {
            // Interactive mode: use readline
            content = readline("> ");
        } else {
            // Non-interactive mode: use get_next_line or fgets
            content = get_next_line(STDIN_FILENO);  // Adjust this line based on what you're using
        }

        if (!content) {
            write(STDERR_FILENO, "warning: here-document delimited by EOF\n", 40);
            break;
        }

        if (ft_strcmp(content, delim) == 0) {
            free(content);
            break;
        }

        expansion = variable_exp_double(content, shell);
        free(content);
        if (!expansion) {
            close(hdfd);
            return (0);
        }

        // Only add newline if in interactive mode
        write(hdfd, expansion, ft_strlen(expansion));

        // Add newline only in interactive mode since get_next_line() may already add it
        if (isatty(STDIN_FILENO)) {
            write(hdfd, "\n", 1);
        }

        free(expansion);
    }
    close(hdfd);
    return (1);
}


// int	write_to_hdfd(char *delim, t_shell *shell, int hdfd)
// {
// 	char	*content;
// 	char	*expansion;

// 	while (1)
// 	{
// 		content = readline("> ");
// 		if (!content)
// 			break ;
// 		if (ft_strcmp(content, delim) == 0)
// 		{
// 			free(content);
// 			break ;
// 		}
// 		expansion = variable_exp_double(content, shell);
// 		free(content);
// 		if (!expansion)
// 		{
// 			return (0);
// 		}
// 		write(hdfd, expansion, ft_strlen(expansion));
// 		write(hdfd, "\n", 1);
// 		free(expansion);
// 	}
// 	close(hdfd);
// 	return (1);
// }

int	validate_heredoc_syntax(t_cmd *cmd, t_token **token, t_shell *shell)
{
	cmd->redir[cmd->redirect_count].type = (*token)->type;
	(*token) = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN_WORD)
	{
		shell->last_exit_status = 2;
		handle_syn_errors(2, "syntax error near redirection\n", shell);
		return (0);
	}
	return (1);
}

char	*create_and_open_temp_file(int redirect_count, int *hered_fd)
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
