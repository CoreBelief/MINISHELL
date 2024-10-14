/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:23:58 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 14:00:57 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_input(char *line, t_shell *shell);
// maybe we move this to different file?
char		*interactive_mode(t_shell *shell);
char		*non_interactive_mode(void);
void		minishell_loop(t_shell *shell);
int			main(int argc, char **argv, char **envp);
// main can be shorter and split up maybe? maybe its fine

static void	process_input(char *line, t_shell *shell)
{
	if (line && *line)
	{
		add_history(line);
		if (!tokenizer(line, shell))
			// all exit codes are set before coming here
			return ;
		// print_token_list(shell->tokens);
		if (!parse_command_from_tokens(shell))
		// all exit codes are set before coming here
		{
			// shell->last_exit_status = 1;
			free_tokens(&shell->tokens);
			return ;
		}
		// print_cmd_list(shell->commands);
		free_tokens(&shell->tokens);
		execute_command(shell);
		free_command_list(&shell->commands);
	}
}

/*MIDDLE*/
// void minishell_loop(t_shell *shell)
// {
//     char *line = NULL;
//     char *prompt = NULL;
//     int original_stdout;
//     int terminal_fd;

//     setup_signals_shell();
//     original_stdout = dup(STDOUT_FILENO); // Keep this,
// // but it may still be problematic.
//     terminal_fd = open("/dev/tty", O_WRONLY);
//     if (terminal_fd == -1)
//     {
//         perror("Failed to open terminal");
//         return ;
//     }
//     while (1)
//     {
//         // dup2(terminal_fd, STDOUT_FILENO);

//         // Commented out isatty checks to force always interactive behavior
//         // if (isatty(STDIN_FILENO))
//         // {
//             prompt = create_prompt();
//             if (!prompt)
//             {
//                 ft_putstr_fd("Error: Failed to create prompt\n",
// terminal_fd);
//                 shell->last_exit_status = 1;
//                 break ;
//             }

//             line = readline(prompt);
//             free(prompt);
//             if (!line)
//             {
//                 // ft_putstr_fd("exit\n", terminal_fd);
//                 break ;
//             }
//             add_history(line);
//         // }
//         // else
//         // {
//         //     line = get_next_line(STDIN_FILENO);
//         //     if (!line)
//         //         break ;
//         //     size_t len = ft_strlen(line);
//         //     if (len > 0 && line[len - 1] == '\n')
//         //         line[len - 1] = '\0';
//         // }

//         // dup2(original_stdout, STDOUT_FILENO);
//         process_input(line, shell);
//         free(line);
//         line = NULL;
//     }
//     if (line)
//         free(line);
//     close(terminal_fd);
//     close(original_stdout);
// }

/*OLDEST*/
// void minishell_loop(t_shell *shell)
// { //function too long needs splitting up!
//     char *line = NULL;
//     char *prompt = NULL;
//     int original_stdout;
//     int terminal_fd;

//     setup_signals_shell();
//     original_stdout = dup(STDOUT_FILENO); //illegalll!!!
//     terminal_fd = open("/dev/tty", O_WRONLY);
//     if (terminal_fd == -1)
//     {
//         perror("Failed to open terminal");
//         return;
//     }
//     while (1)
//     {
//         dup2(terminal_fd, STDOUT_FILENO);
//         if (isatty(STDIN_FILENO))
//         {
//             prompt = create_prompt();
//             if (!prompt)
//             {
//                 ft_putstr_fd("Error: Failed to create prompt\n", terminal_fd);
//                 shell->last_exit_status = 1;
//                 break;
//             }
//             line = readline(prompt);
//             free(prompt);
//             if (!line) 
//             {
//                 ft_putstr_fd("exit\n", terminal_fd);
//                 break;
//             }
//             add_history(line);
//         }
//         else
//         {
//             line = get_next_line(STDIN_FILENO);
//             if (!line)
//                 break;
//             size_t len = ft_strlen(line);
//             if (len > 0 && line[len - 1] == '\n')
//                 line[len - 1] = '\0';
//         }
//         dup2(original_stdout, STDOUT_FILENO);
//         process_input(line, shell);
//         free(line);
//         line = NULL;
//     }
//     if (line)
//         free(line);
//     close(terminal_fd);
//     close(original_stdout);
// }

char	*interactive_mode(t_shell *shell)
{
	char	*line;
	char	*prompt;

	// Interactive mode
	// prompt = NULL;
	prompt = create_prompt();
	if (!prompt)
	{
		perror("Error: Failed to create prompt");
		shell->last_exit_status = 1;
		return (NULL);
	}
	line = readline(prompt);
	free(prompt);
	if (!line)
	{
		printf("exit\n");
		return (NULL);
	}
	if (*line)
		add_history(line);
	return (line);
}

char	*non_interactive_mode(void)
{
	char		*line;
	size_t		len;

	// Non-interactive mode
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	// Remove newline character if present
	len = strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

/* LATEST*/
void	minishell_loop(t_shell *shell)
{
	char	*line;

	line = NULL;
	setup_signals_shell();
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			line = interactive_mode(shell);
			if (!line)
				break ;
		}
		else
		{
			line = non_interactive_mode();
			if (!line)
				break ;
		}
		// if (*line)
		process_input(line, shell);
		free(line);
		line = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	init_shell(&shell);
	if (!init_env(&shell, envp))
		return (perror("error init env"), EXIT_FAILURE);
	if (!increment_shlvl(&shell))
		return (EXIT_FAILURE);
	if (argc > 1 && strcmp(argv[1], "-c") == 0)
	{
		if (argc > 2)
		{
			process_input(argv[2], &shell);
			free_shell(&shell);
			return (shell.last_exit_status);
		}
		else
		{
			ft_putendl_fd("Error: -c requires an argument", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	minishell_loop(&shell);
	free_shell(&shell);
	return (shell.last_exit_status);
}
