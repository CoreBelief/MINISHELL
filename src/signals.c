#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t g_exit_status = 0;

static void handle_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_exit_status = 130;  // 128 + SIGINT (2)
}

static void handle_sigquit(int sig)
{
    (void)sig;
    rl_on_new_line();
    rl_redisplay();
}

static int setup_sigaction(int signum, void (*handler)(int))
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    return sigaction(signum, &sa, NULL);
}

void setup_signals_shell(void)
{
    if (setup_sigaction(SIGINT, handle_sigint) == -1)
        perror("Error setting up SIGINT handler");
    if (setup_sigaction(SIGQUIT, handle_sigquit) == -1)
        perror("Error setting up SIGQUIT handler");
    if (signal(SIGTERM, SIG_IGN) == SIG_ERR)
        perror("Error ignoring SIGTERM");
}

void setup_signals_child(void)
{
    if (signal(SIGINT, SIG_DFL) == SIG_ERR)
        perror("Error setting SIGINT to default in child");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
        perror("Error setting SIGQUIT to default in child");
    if (signal(SIGTERM, SIG_DFL) == SIG_ERR)
        perror("Error setting SIGTERM to default in child");
}

void handle_signals_during_execution(void)
{
    if (g_exit_status == 130)  // SIGINT
    {
        write(STDERR_FILENO, "\n", 1);
    }
    else if (g_exit_status == 131)  // SIGQUIT
    {
        write(STDERR_FILENO, "Quit (core dumped)\n", 19);
    }
}

int check_eof(char *line)
{
    if (line == NULL)
    {
        write(STDERR_FILENO, "exit\n", 5);
        return 1;
    }
    return 0;
}

int get_exit_status(void)
{
    return g_exit_status;
}

void reset_exit_status(void)
{
    g_exit_status = 0;
}
void reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    // Add any other signals you've modified
}

// Example usage (can be removed in actual implementation)


// #define _POSIX_C_SOURCE 200809L
//     #include "minishell.h"


//     extern int	g_exit_status;

//     static void	handle_sigint(int sig)
//     {
//         (void)sig;
//         write(STDOUT_FILENO, "\n", 1);
//         rl_on_new_line();
//         rl_replace_line("", 0);
//         rl_redisplay();
//         g_exit_status = 1;
//     }

//     static void	handle_sigquit(int sig)
//     {
//         (void)sig;
//         rl_on_new_line();
//         rl_redisplay();
//     }

//     static int	setup_sigaction(int signum, void (*handler)(int))
//     {
//         struct sigaction	sa;

//         sa.sa_handler = handler;
//         sa.sa_flags = SA_RESTART;
//         sigemptyset(&sa.sa_mask);
//         return (sigaction(signum, &sa, NULL));
//     }

//     void	setup_signals(void)
//     {
//         if (setup_sigaction(SIGINT, handle_sigint) == -1)
//             perror("Error setting up SIGINT handler");
//         if (setup_sigaction(SIGQUIT, handle_sigquit) == -1)
//             perror("Error setting up SIGQUIT handler");
//         if (signal(SIGTERM, SIG_IGN) == SIG_ERR)
//             perror("Error ignoring SIGTERM");
//     }









// //     #include "minishell.h"

// // volatile sig_atomic_t g_signal_status = 0;

// // /* Handle SIGINT (Ctrl-C) signal */
// // static void handle_sig_int(int sig)
// // {
// //     (void)sig;
// //     g_signal_status = SIGINT;
// //     write(STDERR_FILENO, "\n", 1);
// //     rl_on_new_line();
// //     rl_replace_line("", 0);
// //     rl_redisplay();
// // }

// // /* Handle SIGQUIT (Ctrl-\) signal commentere out for now*/
// // // static void handle_sig_quit(int sig)
// // // {
// // //     (void)sig;
// // //     g_signal_status = SIGQUIT;
// // // }

// // /* Set up signal action for a given signal number */
// // static int set_up_sig_action(int signum, void (*handler)(int))
// // {
// //     struct sigaction sa;

// //     sa.sa_handler = handler;
// //     sa.sa_flags = SA_RESTART;
// //     sigemptyset(&sa.sa_mask);
// //     return (sigaction(signum, &sa, NULL));
// // }

// // /* Set up all required signal handlers for the shell */
// // void setup_signals_shell(void)
// // {
// //     if (set_up_sig_action(SIGINT, handle_sig_int) == -1)
// //         // handle_error("Error setting up SIGINT handler");
// //         printf("Error setting up SIGINT handler");

// //     if (set_up_sig_action(SIGQUIT, SIG_IGN) == -1)
// //         // handle_error("Error ignoring SIGQUIT");
// //         printf("Error ignoring SIGQUIT");

// //     if (signal(SIGTERM, SIG_IGN) == SIG_ERR)
// //         // handle_error("Error ignoring SIGTERM");
// //         printf("Error ignoring SIGTERM");

// // }

// // /* Set up signal handlers for child processes */
// // void setup_signals_child(void)
// // {
// //     if (signal(SIGINT, SIG_DFL) == SIG_ERR)
// //         // handle_error("Error setting SIGINT to default in child");
// //         printf("Error setting SIGINT to default in child");

// //     if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
// //         // handle_error("Error setting SIGQUIT to default in child");
// //         printf("Error setting SIGQUIT to default in child");

// //     if (signal(SIGTERM, SIG_DFL) == SIG_ERR)
// //         // handle_error("Error setting SIGTERM to default in child");
// //         printf("Error setting SIGTERM to default in child");
// // }

// // /* Handle signals during command execution */
// // void handle_signals_during_execution(void)
// // {
// //     if (g_signal_status == SIGINT)
// //     {
// //         write(STDERR_FILENO, "\n", 1);
// //         g_signal_status = 130;
// //     }
// //     else if (g_signal_status == SIGQUIT)
// //     {
// //         write(STDERR_FILENO, "Quit (core dumped)\n", 19);
// //         g_signal_status = 131;
// //     }
// // }

// // /* Check for EOF (Ctrl-D) */
// // int check_eof(char *line)
// // {
// //     if (line == NULL)
// //     {
// //         write(STDERR_FILENO, "exit\n", 5);
// //         return (1);
// //     }
// //     return (0);
// // }

// // /* Get the current exit status */
// // int get_exit_status(void)
// // {
// //     if (g_signal_status == SIGINT || g_signal_status == SIGQUIT)
// //     {
// //         int temp = g_signal_status;
// //         g_signal_status = 0;
// //         return temp;
// //     }
// //     return g_signal_status;
// // }

// // /* Reset the signal status */
// // void reset_signal_status(void)
// // {
// //     g_signal_status = 0;
// // }