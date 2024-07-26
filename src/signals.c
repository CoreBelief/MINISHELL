#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>

void handle_sigint(int sig)
{
    (void)sig;
    printf("DEBUG: SIGINT received\n");
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_exit_status = 1;
    printf("DEBUG: SIGINT handler completed\n");
}

void handle_sigquit(int sig)
{
    (void)sig;
    printf("DEBUG: SIGQUIT received\n");
    // Do nothing for SIGQUIT in interactive mode
    printf("DEBUG: SIGQUIT handler completed\n");
}

void setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    printf("DEBUG: Setting up signal handlers\n");

    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
    {
        perror("DEBUG: Error setting up SIGINT handler");
    }
    else
    {
        printf("DEBUG: SIGINT handler set up successfully\n");
    }

    sa_quit.sa_handler = handle_sigquit;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
    {
        perror("DEBUG: Error setting up SIGQUIT handler");
    }
    else
    {
        printf("DEBUG: SIGQUIT handler set up successfully\n");
    }

    // Ignore SIGTERM
    if (signal(SIGTERM, SIG_IGN) == SIG_ERR)
    {
        perror("DEBUG: Error ignoring SIGTERM");
    }
    else
    {
        printf("DEBUG: SIGTERM ignored successfully\n");
    }

    printf("DEBUG: Signal setup completed\n");
}