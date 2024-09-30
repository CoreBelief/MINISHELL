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
#include <fcntl.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

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
    char *line = NULL;
    char *prompt = NULL;
    int original_stdout;
    int terminal_fd;

    setup_signals_shell();

    // Save the original stdout
    original_stdout = dup(STDOUT_FILENO);
    
    // Open the controlling terminal
    terminal_fd = open("/dev/tty", O_WRONLY);
    if (terminal_fd == -1) {
        perror("Failed to open terminal");
        return;
    }

    while (1)
    {
        // Restore stdout to the terminal for readline
        dup2(terminal_fd, STDOUT_FILENO);

        if (isatty(STDIN_FILENO))
        {
            // Interactive mode
            prompt = create_prompt();
            if (!prompt)
            {
                ft_putstr_fd("Error: Failed to create prompt\n", terminal_fd);
                shell->last_exit_status = 1;
                break;
            }
            
            line = readline(prompt);
            free(prompt);

            if (!line) // EOF (Ctrl+D)
            {
                ft_putstr_fd("exit\n", terminal_fd);
                break;
            }

            add_history(line);
        }
        else
        {
            // Non-interactive mode
            line = get_next_line(STDIN_FILENO);
            if (!line) // EOF or error
                break;

            // Remove newline character if present
            size_t len = ft_strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';
        }

        // Restore original stdout for command execution
        dup2(original_stdout, STDOUT_FILENO);

        // Process the input line
        process_input(line, shell);

        free(line);
        line = NULL;
    }

    if (line)
        free(line);
    
    close(terminal_fd);
    close(original_stdout);
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

void ft_custom_itoa(int n, char *str, int max_len) {
    int i = 0;
    int is_negative = 0;
    
    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    
    if (n < 0) {
        is_negative = 1;
        n = -n;
    }
    
    while (n != 0 && i < max_len - 1) {
        str[i++] = (n % 10) + '0';
        n = n / 10;
    }
    
    if (is_negative && i < max_len - 1) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
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
    ft_custom_itoa(shlvl, new_shlvl, sizeof(new_shlvl));  // Convert int to string using custom function

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

