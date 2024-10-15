/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   interaction_mode.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/15 17:29:05 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 18:50:58 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			open_terminal(void);
static char	*get_interactive_input(int terminal_fd, t_shell *shell);
static char	*get_non_interactive_input(void);
void		cleanup(int terminal_fd, int original_stdout);
char		*get_input_line(int terminal_fd, t_shell *shell);

int	open_terminal(void)
{
	int	terminal_fd;

	terminal_fd = open("/dev/tty", O_WRONLY);
	if (terminal_fd == -1)
	{
		perror("Failed to open terminal");
	}
	return (terminal_fd);
}

static char	*get_interactive_input(int terminal_fd, t_shell *shell)
{
	char	*prompt;
	char	*line;

	prompt = create_prompt();
	if (!prompt)
	{
		ft_putstr_fd("Error: Failed to create prompt\n", terminal_fd);
		shell->last_exit_status = 1;
		return (NULL);
	}
	line = readline(prompt);
	free(prompt);
	if (!line)
	{
		ft_putstr_fd("exit\n", terminal_fd);
	}
	return (line);
}

static char	*get_non_interactive_input(void)
{
	char	*line;
	size_t	len;

	line = get_next_line(STDIN_FILENO);
	if (line)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
	}
	return (line);
}

void	cleanup(int terminal_fd, int original_stdout)
{
	close(terminal_fd);
	close(original_stdout);
}

char	*get_input_line(int terminal_fd, t_shell *shell)
{
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		line = get_interactive_input(terminal_fd, shell);
	}
	else
	{
		line = get_non_interactive_input();
	}
	return (line);
}
