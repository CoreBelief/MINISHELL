// #define _POSIX_C_SOURCE 200809L
// #include <signal.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include "signals.h"
// #include <errno.h>
// #include <sys/wait.h>

// volatile sig_atomic_t g_received_signal = 0;

// static void handle_signal(int sig)
// {
//     g_received_signal = sig;
//     if (sig == SIGINT) {
//         write(STDOUT_FILENO, "\n", 1);
//         rl_on_new_line();
//         rl_replace_line("", 0);
//         rl_redisplay();
//     }
// }

// static void sigchld_handler(int signum)
// {
//     (void)signum;  // Suppress unused parameter warning
//     while (waitpid(-1, NULL, WNOHANG) > 0);
// }

// static int setup_signal(int signum, void (*handler)(int))
// {
//     struct sigaction sa;
//     sa.sa_handler = handler;
//     sa.sa_flags = SA_RESTART;
//     sigemptyset(&sa.sa_mask);  // Block all signals during the handler
//     return sigaction(signum, &sa, NULL);
// }

// void setup_signals_shell(void)
// {
//     if (setup_signal(SIGINT, handle_signal) == -1) {
//         perror("Error setting up SIGINT handler");
//         exit(EXIT_FAILURE);
//     }
//     if (setup_signal(SIGQUIT, SIG_IGN) == -1) {
//         perror("Error ignoring SIGQUIT");
//         exit(EXIT_FAILURE);
//     }
//     if (setup_signal(SIGTERM, SIG_IGN) == -1) {
//         perror("Error ignoring SIGTERM");
//         exit(EXIT_FAILURE);
//     }
//     if (setup_signal(SIGTSTP, SIG_DFL) == -1) {
//         perror("Error ignoring SIGTSTP");
//         exit(EXIT_FAILURE);
//     }
//     if (setup_signal(SIGCHLD, sigchld_handler) == -1) {
//         perror("Error setting up SIGCHLD handler");
//         exit(EXIT_FAILURE);
//     }
// }

// void setup_signals_child(void) {
//     struct sigaction sa;
//     sa.sa_flags = SA_RESTART;
//     sigemptyset(&sa.sa_mask);

//     // SIGINT: Custom handler
//     sa.sa_handler = SIG_DFL;
//     if (sigaction(SIGINT, &sa, NULL) == -1) {
//         perror("Error setting SIGINT handler in child");
//         exit(EXIT_FAILURE);
//     }

//     // SIGQUIT and SIGTERM: Default action
//     sa.sa_handler = SIG_DFL;
//     if (sigaction(SIGQUIT, &sa, NULL) == -1 || sigaction(SIGTERM, &sa, NULL) == -1) {
//         perror("Error setting SIGQUIT/SIGTERM to default in child");
//         exit(EXIT_FAILURE);
//     }
// }

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


static void sigchld_handler(int signum)
{
    (void)signum; 
    while (waitpid(-1, NULL, WNOHANG) > 0);  
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

    if (setup_signal(SIGCHLD, sigchld_handler, SA_RESTART) == -1)  
    {
        perror("Error setting up SIGCHLD handler");
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

