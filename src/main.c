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
		tokens = tokenizer(line, shell);
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
        if (isatty(fileno(stdin)))
        {
            // Interactive mode: create and display the prompt
            prompt = create_prompt();
            if (!prompt)
            {
                ft_putendl_fd("Error: Failed to create prompt", STDERR_FILENO);
                shell->last_exit_status = 1;
                break;
            }
            line = readline(prompt);
            free(prompt);
        }
        else
        {
            // Non-interactive mode: read input using get_next_line
            line = get_next_line(fileno(stdin));
            if (line)
            {
                // Remove newline character if present
                size_t len = ft_strlen(line);
                if (len > 0 && line[len - 1] == '\n')
                    line[len - 1] = '\0';
            }
        }

        // If no input (EOF or error)
        if (!line)
        {
            if (isatty(fileno(stdin)))
                ft_putendl_fd("exit", STDOUT_FILENO); // Print "exit" in interactive mode
            break;
        }

        // Process the input line
        process_input(line, shell);

        free(line); // Free the input line after processing
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

void increment_shlvl(t_shell *shell) {
    char *shlvl_str = ft_get_env("SHLVL", shell);  // Fetch the current SHLVL value
    int shlvl = 1;  // Default to 1 if not found

    if (shlvl_str != NULL) {
        shlvl = ft_atoi(shlvl_str);  // Convert string to int
        shlvl += 1;  // Increment the SHLVL value
    }

    // Create a string for the new SHLVL value
    char new_shlvl[16];  // Assuming 16 bytes is enough for the integer + null terminator
    snprintf(new_shlvl, sizeof(new_shlvl), "%d", shlvl); // have to change this line

    // Update the SHLVL environment variable
    ft_set_env("SHLVL", new_shlvl, shell);  // 1 to overwrite any existing value
}


int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	
	init_shell(&shell);
	if (!init_env(&shell, envp))
	{
		perror("error init env");
		return (EXIT_FAILURE);
	}
    increment_shlvl(&shell);

	if (argc > 1 && strcmp(argv[1], "-c") == 0)
	{
		// Non-interactive mode with -c: execute the command given in argv[2]
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

	// Interactive or script mode
	minishell_loop(&shell);
	free_shell(&shell);
	return (shell.last_exit_status);
}

