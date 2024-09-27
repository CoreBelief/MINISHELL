#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "environ.h"
#include "signal.h"

static void	process_input(char *line, t_shell *shell)
{
	t_token		*tokens;

	if (line && *line)
	{
		add_history(line);
		tokens = tokenizer(line);
		if (!tokens)
		{
			free_tokens(&tokens);
			shell->last_exit_status = 1;
			return ;
		}
		shell->commands = parse_command_from_tokens(tokens, shell);
		if (!shell->commands)
		{
			shell->last_exit_status = 1;
			free_tokens(&tokens);
			free_command_list(&shell->commands);
			return ;
		}
		free_tokens(&tokens);
		execute_command(shell);
		free_command_list(&shell->commands);
	}
}





void minishell_loop(t_shell *shell)
{
    char *line;
    char *prompt;

    setup_signals_shell();

    while (1)
    {
        // Create the prompt
        prompt = create_prompt();
        if (!prompt)
        {
            ft_putendl_fd("Error: Failed to create prompt", STDERR_FILENO);
            shell->last_exit_status = 1; // Error creating prompt, set exit code to 1
            break;
        }

        // Handle input based on whether it's from a terminal (interactive) or not (script)
        if (isatty(fileno(stdin)))
        {
            // Interactive mode: read input using readline
            line = readline(prompt);
        }
        else
        {
            // Non-interactive mode: read input from script or file
            line = readline(prompt); // Still using readline to maintain consistency
        }

        free(prompt); // Free the prompt after it's used

        // If no input (EOF or error)
        if (!line)
        {
            // For interactive mode, exit with status 0
            if (isatty(fileno(stdin)))
            {
                shell->last_exit_status = 0; // In interactive mode, return 0 when exiting
            }
            else
            {
                // In non-interactive mode, keep the last exit status as it was
                // Do not set exit status to 1 unless there was an actual error
            }

            ft_putendl_fd("exit", STDOUT_FILENO);
            break;
        }

        // Process the input line (should update shell->last_exit_status based on the command's outcome)
        process_input(line, shell);

        free(line); // Free the input line after processing
    }

    // Ensure that when we reach the end of a non-interactive session (script), the exit code is correct
    if (!isatty(fileno(stdin)) && shell->last_exit_status == 0)
    {
        // If no errors occurred in non-interactive mode (like in a script), exit with status 0
        shell->last_exit_status = 0;
    }
}





void	free_shell(t_shell *shell)
{
	if (shell->env)
	{
		ft_free_str_array(shell->env);
	}
}

void	init_shell(t_shell *shell)
{
	shell->commands = NULL;
	shell->last_exit_status = 0;
	shell->env = NULL;
	shell->env_size = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;

	init_shell(&shell);
	if (!init_env(&shell, envp))
	{
		perror("error init env");
		return (EXIT_FAILURE);
	}
	minishell_loop(&shell);
	free_shell(&shell);
	return (shell.last_exit_status);
}

