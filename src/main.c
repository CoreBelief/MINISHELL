/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:23:58 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 15:00:00 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_input(char *line, t_shell *shell)
{
	if (line && *line)
	{
		add_history(line);
		if (!tokenizer(line, shell))
			return ;
		if (!parse_command_from_tokens(shell))
		{
			free_tokens(&shell->tokens);
			return ;
		}
		free_tokens(&shell->tokens);
		execute_command(shell);
		free_command_list(&shell->commands);
	}
}

int open_terminal(void)
{
    int terminal_fd = open("/dev/tty", O_WRONLY);
    if (terminal_fd == -1)
    {
        perror("Failed to open terminal");
    }
    return terminal_fd;
}

char *get_interactive_input(int terminal_fd, t_shell *shell)
{
    char *prompt = create_prompt();
    if (!prompt)
    {
        ft_putstr_fd("Error: Failed to create prompt\n", terminal_fd);
        shell->last_exit_status = 1;
        return NULL;
    }

    char *line = readline(prompt);
    free(prompt);

    if (!line)
    {
        ft_putstr_fd("exit\n", terminal_fd);
    }
    return line;
}

char *get_non_interactive_input(void)
{
    char *line = get_next_line(STDIN_FILENO);
    if (line)
    {
        size_t len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
    }
    return line;
}

void cleanup(int terminal_fd, int original_stdout)
{
    close(terminal_fd);
    close(original_stdout);
}

char *get_input_line(int terminal_fd, t_shell *shell)
{
    char *line;
    if (isatty(STDIN_FILENO))
    {
        line = get_interactive_input(terminal_fd, shell);
    }
    else
    {
        line = get_non_interactive_input();
    }
    return line;
}

void minishell_loop(t_shell *shell)
{
    int original_stdout = dup(STDOUT_FILENO);
    int terminal_fd = open_terminal();
    if (terminal_fd == -1)
        return;

    setup_signals_shell();

    while (1)
    {
        dup2(terminal_fd, STDOUT_FILENO);
        char *line = get_input_line(terminal_fd, shell);
        if (!line)
            break;

        dup2(original_stdout, STDOUT_FILENO);
        process_input(line, shell);
        free(line);
    }

    cleanup(terminal_fd, original_stdout);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	init_shell(&shell);
	if (!init_env(&shell, envp))
		return (ft_putendl_fd("Error: init env failed", STDERR_FILENO), \
				EXIT_FAILURE);
	if (!increment_shlvl(&shell))
		return (EXIT_FAILURE);
	if (argc > 1 && ft_strcmp(argv[1], "-c") == 0)
	{
		if (argc > 2)
		{
			process_input(argv[2], &shell);
			free_shell(&shell);
			return (shell.last_exit_status);
		}
		else
			return (ft_putendl_fd("Error: -c requires an argument", \
					STDERR_FILENO), EXIT_FAILURE);
	}
	minishell_loop(&shell);
	free_shell(&shell);
	return (shell.last_exit_status);
}