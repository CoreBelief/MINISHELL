#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "signals.h"
#include <errno.h>
#include <sys/wait.h>

volatile sig_atomic_t g_received_signal = 0;

static void handle_signal(int sig)
{
    g_received_signal = sig;
    if (sig == SIGINT) {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

static void sigchld_handler(int signum)
{
    (void)signum;  // Suppress unused parameter warning
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

static int setup_signal(int signum, void (*handler)(int))
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);  // Block all signals during the handler
    return sigaction(signum, &sa, NULL);
}
void setup_signals_shell(void)
{
    if (setup_signal(SIGINT, handle_signal) == -1) {
        perror("Error setting up SIGINT handler");
        exit(EXIT_FAILURE);
    }
    if (setup_signal(SIGQUIT, SIG_IGN) == -1) {
        perror("Error ignoring SIGQUIT");
        exit(EXIT_FAILURE);
    }
    if (setup_signal(SIGTERM, SIG_IGN) == -1) {
        perror("Error ignoring SIGTERM");
        exit(EXIT_FAILURE);
    }
    if (setup_signal(SIGTSTP, SIG_IGN) == -1) {
        perror("Error ignoring SIGTSTP");
        exit(EXIT_FAILURE);
    }

    // Optionally add SIGCHLD handling if needed
    if (setup_signal(SIGCHLD, sigchld_handler) == -1) {
        perror("Error setting up SIGCHLD handler");
        exit(EXIT_FAILURE);
    }
}

void setup_signals_child(void) {
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);

    // SIGINT: Custom handler
    sa.sa_handler = handle_signal;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "Error setting SIGINT handler in child: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // SIGQUIT and SIGTERM: Default action
    sa.sa_handler = SIG_DFL;
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        fprintf(stderr, "Error setting SIGQUIT to default in child: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        fprintf(stderr, "Error setting SIGTERM to default in child: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // SIGTSTP: Ignore
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        fprintf(stderr, "Error setting SIGTSTP to ignore in child: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Optional: Setup any other signals you want to handle in child processes
}
// void setup_signals_child(void) {
//     struct sigaction sa;
//     sa.sa_flags = SA_RESTART;
//     sigemptyset(&sa.sa_mask);

//     // SIGINT: Custom handler
//     sa.sa_handler = handle_signal;
//     if (sigaction(SIGINT, &sa, NULL) == -1) {
//         perror("Error setting SIGINT handler in child");
//         exit(EXIT_FAILURE);
//     }

//     // SIGQUIT and SIGTERM: Default action
//     sa.sa_handler = SIG_DFL;
//     if (sigaction(SIGQUIT, &sa, NULL) == -1 || sigaction(SIGTERM, &sa, NULL) == -1) {
//         perror("Error setting SIGQUIT or SIGTERM to default in child");
//         exit(EXIT_FAILURE);
//     }

//     // SIGTSTP: Ignore
//     sa.sa_handler = SIG_IGN;
//     if (sigaction(SIGTSTP, &sa, NULL) == -1) {
//         perror("Error setting SIGTSTP to ignore in child");
//         exit(EXIT_FAILURE);
//     }
// }

int get_and_reset_signal(void)
{
    int sig = g_received_signal;
    g_received_signal = 0;
    return sig;
}

