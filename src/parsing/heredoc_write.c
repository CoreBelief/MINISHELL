/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_write.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/18 14:22:09 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/18 15:04:30 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_content(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	return (get_next_line(STDIN_FILENO));
}

static int	check_and_write(char *str, int hdfd)
{
	if (!str)
		return (0);
	write(hdfd, str, ft_strlen(str));
	if (isatty(STDIN_FILENO))
		write(hdfd, "\n", 1);
	return (1);
}

static int	process_content(char *content, t_shell *shell, int hdfd)
{
	char	*expansion;
	int		result;

	expansion = variable_exp_double(content, shell);
	free(content);
	result = check_and_write(expansion, hdfd);
	free(expansion);
	if (!result)
	{
		close(hdfd);
		return (0);
	}
	return (1);
}

int	write_to_hdfd(char *delim, t_shell *shell, int hdfd)
{
	char	*content;

	while (1)
	{
		content = get_content();
		if (!content)
		{
			write(STDERR_FILENO, "warning: here-document at EOF\n", 29);
			break ;
		}
		if (ft_strcmp(content, delim) == 0)
		{
			free(content);
			break ;
		}
		if (!process_content(content, shell, hdfd))
			return (0);
	}
	close(hdfd);
	return (1);
}
