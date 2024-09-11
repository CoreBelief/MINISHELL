#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "signals.h"

volatile sig_atomic_t g_received_signal = 0;

static void handle_signal(int sig)
{
	g_received_signal = sig;
	if (sig == SIGINT) {
		// write(STDOUT_FILENO, "^C\n", 3); idiot fixx because i lost it....
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static int setup_signal(int signum, void (*handler)(int))
{
	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	return sigaction(signum, &sa, NULL);
}

void setup_signals_shell(void)
{
	if (setup_signal(SIGINT, handle_signal) == -1)
		perror("Error setting up SIGINT handler");
	if (setup_signal(SIGQUIT, SIG_IGN) == -1)
		perror("Error setting up SIGQUIT handler");
	if (setup_signal(SIGTERM, SIG_IGN) == -1)
		perror("Error ignoring SIGTERM");
	if (setup_signal(SIGTSTP, SIG_IGN) == -1)
		perror("Error ignoring SIGTSTP");
}

void setup_signals_child(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		perror("Error setting SIGINT to default in child");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		perror("Error setting SIGQUIT to default in child");
	if (signal(SIGTERM, SIG_DFL) == SIG_ERR)
		perror("Error setting SIGTERM to default in child");
	if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
		perror("Error setting SIGTSTP to default in child");
}

int get_and_reset_signal(void)
{
    int sig = g_received_signal;
    g_received_signal = 0;
    return sig;
}

void handle_pending_signals(void)
{
    // fix this function since it =s only wokring correctly on sigquit!!
    int sig = get_and_reset_signal();
    if (sig == SIGINT) {
        // write(STDOUT_FILENO, "\n", 1);  // Print newline after interrupt
    }
    else if (sig == SIGQUIT) {
        write(STDERR_FILENO, "Quit\n", 6);
    }
}
