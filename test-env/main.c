#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

int g_exit_status = 0;

static void	handle_signals(void)
{
	// TODO: Implement signal handling
    printf("TODOO HANDLE SIGNALS!");
}

static void	process_input(char *line)
{
	if (line && *line)
	{
		add_history(line);
		execute_command(line);
	}
}

void	minishell_loop(void)
{
	char	*line;

	handle_signals();
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		process_input(line);
		free(line);
	}
}

int	main(void)
{
	minishell_loop();
	return (g_exit_status);
}