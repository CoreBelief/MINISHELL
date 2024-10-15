/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:17:48 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 14:17:12 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

#include "minishell.h"

# define SIGINT_EXIT_STATUS 130
# define SIGQUIT_EXIT_STATUS 131



int								setup_signal(int signum, void (*handler)(int),
									int flags);

void							setup_signals_shell(void);

void							setup_signals_child(void);




#endif /* SIGNALS_H */