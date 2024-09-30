/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rdl <rdl@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 19:24:35 by rdl           #+#    #+#                 */
/*   Updated: 2024/09/30 16:34:30 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <errno.h>

volatile sig_atomic_t g_received_signal = 0;

void suppress_broken_pipe_message(void);
static void handle_signal(int sig);
static int setup_signal(int signum, void (*handler)(int), int flags);
void setup_signals_shell(void);
void setup_signals_child(void);


void suppress_broken_pipe_message(void)
{ //not sure if this pipehandling is beneficial, we could test if we should remove thisremove
    int saved_errno = errno;
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
        perror("Error setting up SIGPIPE handler");
        exit(EXIT_FAILURE);
    }
    errno = saved_errno;
}

static void handle_signal(int sig)
{
    g_received_signal = sig;
    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);  
        rl_on_new_line();               
        rl_replace_line("", 0);         
        rl_redisplay();                 
    }
}


static int setup_signal(int signum, void (*handler)(int), int flags)
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = flags;
    sigemptyset(&sa.sa_mask);  
    return sigaction(signum, &sa, NULL); 
}


void setup_signals_shell(void)
{   
    suppress_broken_pipe_message(); //if pipe handling is removed this has to go too!
    if (setup_signal(SIGINT, handle_signal, SA_RESTART) == -1)  
    {
        perror("Error setting up SIGINT handler");
        exit(EXIT_FAILURE);
    }

    if (setup_signal(SIGQUIT, SIG_IGN, 0) == -1)  
    {
        perror("Error ignoring SIGQUIT");
        exit(EXIT_FAILURE);
    }
    if (setup_signal(SIGTERM, SIG_IGN, 0) == -1)  
    {
        perror("Error ignoring SIGTERM");
        exit(EXIT_FAILURE);
    }

    if (setup_signal(SIGTSTP, SIG_DFL, 0) == -1)  
    {
        perror("Error setting up SIGTSTP handler");
        exit(EXIT_FAILURE);
    }
}


void setup_signals_child(void)
{
    if (setup_signal(SIGINT, SIG_DFL, 0) == -1 ||  
        setup_signal(SIGQUIT, SIG_DFL, 0) == -1 || 
        setup_signal(SIGTERM, SIG_DFL, 0) == -1)   
    {
        perror("Error setting signal handlers in child");
        exit(EXIT_FAILURE);
    }
}

