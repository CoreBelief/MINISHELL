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
		// print_token_list(tokens);
		shell->commands = parse_command_from_tokens(tokens, shell);
		if (!shell->commands)
		{
			shell->last_exit_status = 1;
			free_tokens(&tokens);
			free_command_list(&shell->commands);
			return ;
		}
		// print_cmd_list(shell->commands);
		free_tokens(&tokens);
		execute_command(shell);
		free_command_list(&shell->commands);
	}
}


void	minishell_loop(t_shell *shell)
{
	char	*line;
	char	*prompt;

	setup_signals_shell();
	while (1)
	{
		prompt = create_prompt();
		if (!prompt)
		{
			// fprintf(stderr, "Error: Failed to create prompt\n"); //cannot use fprintf
			write(STDERR_FILENO, "Error: Failed to create prompt\n", 31);
            shell->last_exit_status = 1; 
			break ;
		}
		line = readline(prompt);
		free(prompt);
		if (!line)
		{	
			shell->last_exit_status = 0;
			printf("exit\n");
			break ;
		}
		process_input(line, shell);
		free(line);
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

