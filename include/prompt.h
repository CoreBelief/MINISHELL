/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:19:46 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 17:19:53 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# include "environ.h"
# include "minishell.h"
# include "signal.h"
# include <pwd.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_HOSTNAME 256
# define MAX_USERNAME 256
# define MAX_PATH 1024

char	*create_prompt(void);

#endif // PROMPT_H
