/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:19:46 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 19:05:05 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# include "minishell.h"

# define MAX_HOSTNAME 256
# define MAX_USERNAME 256
# define MAX_PATH 1024

char	*create_prompt(void);
char	*get_username(void);
char	*get_current_dir(void);
char	*get_parent_dir(char *path);

#endif
