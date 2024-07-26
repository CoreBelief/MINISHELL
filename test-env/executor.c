#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>

static void	child_process(char **args)
{
	printf("external: %s\n", args[0]);
	execvp(args[0], args);
	perror("minishell");
	exit(EXIT_FAILURE);
}

static int	is_builtin(char *cmd)
{
	// This is a placeholder. Implement actual builtin check.
	return (0);
}

static void	execute_builtin(char **args)
{
	// This is a placeholder. Implement actual builtin execution.
	printf("internal: %s\n", args[0]);
}

void	execute_command(char *command)
{
	pid_t	pid;
	int		status;
	char	**args;

	args = tokenize_command(command);
	if (!args)
		return ;
	if (is_builtin(args[0]))
		execute_builtin(args);
	else
	{
		pid = fork();
		if (pid == -1)
			perror("minishell");
		else if (pid == 0)
			child_process(args);
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
		}
	}
	free(args);
}