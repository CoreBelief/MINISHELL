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

int g_exit_status = 0;

void update_exit_status(int status)
{
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        g_exit_status = 128 + WTERMSIG(status);
}


static void	process_input(char *line, t_shell *shell)
{
	t_token		*tokens;

	if (line && *line)
	{
		add_history(line);
		tokens = tokenizer(line);
		if (!tokens)
			return ;
		shell->commands = parse_command_from_tokens(tokens, shell);
		if (!shell->commands)
		{
			free_tokens(&tokens);
			return ;
		}
		free_tokens(&tokens);
		// print_cmd_list(shell->commands);
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
			fprintf(stderr, "Error: Failed to create prompt\n"); //cannot use fprintf
			break ;
		}
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		process_input(line, shell);
		free(line);
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
		return (1);
	}
	minishell_loop(&shell);
	return (shell.last_exit_status);
}

