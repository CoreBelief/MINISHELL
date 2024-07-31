#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

extern int g_exit_status;

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 1;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	// Do nothing for SIGQUIT in interactive mode
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		perror("Error setting up SIGINT handler");

	sa_quit.sa_handler = handle_sigquit;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		perror("Error setting up SIGQUIT handler");

	if (signal(SIGTERM, SIG_IGN) == SIG_ERR)
		perror("Error ignoring SIGTERM");
}
