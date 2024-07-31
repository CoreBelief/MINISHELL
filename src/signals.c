    /* ************************************************************************** */
    /*                                                                            */
    /*                                                        :::      ::::::::   */
    /*   signals.c                                          :+:      :+:    :+:   */
    /*                                                    +:+ +:+         +:+     */
    /*   By: [Your Username] <[your@email.com]>          +#+  +:+       +#+        */
    /*                                                +#+#+#+#+#+   +#+           */
    /*   Created: [Current Date] by [Your Username]      #+#    #+#             */
    /*   Updated: [Current Date] by [Your Username]     ###   ########.fr       */
    /*                                                                            */
    /* ************************************************************************** */

    #include "minishell.h"
    #include <stdio.h>
    #include <readline/readline.h>
    #include <signal.h>
    #include <unistd.h>

    extern int	g_exit_status;

    static void	handle_sigint(int sig)
    {
        (void)sig;
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_exit_status = 1;
    }

    static void	handle_sigquit(int sig)
    {
        (void)sig;
        rl_on_new_line();
        rl_redisplay();
    }

    static int	setup_sigaction(int signum, void (*handler)(int))
    {
        struct sigaction	sa;

        sa.sa_handler = handler;
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sa.sa_mask);
        return (sigaction(signum, &sa, NULL));
    }

    void	setup_signals(void)
    {
        if (setup_sigaction(SIGINT, handle_sigint) == -1)
            perror("Error setting up SIGINT handler");
        if (setup_sigaction(SIGQUIT, handle_sigquit) == -1)
            perror("Error setting up SIGQUIT handler");
        if (signal(SIGTERM, SIG_IGN) == SIG_ERR)
            perror("Error ignoring SIGTERM");
    }