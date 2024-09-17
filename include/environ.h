/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   environ.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/17 13:41:39 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/17 13:41:47 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_H
# define ENVIRON_H

# include "minishell.h"
# include "structs.h"

int		init_env(t_shell *shell, char **envp);

char	*ft_get_env(const char *name, t_shell *shell);

int		ft_set_env(const char *name, const char *value, t_shell *shell);

int		ft_unset_env(const char *name, t_shell *shell);

#endif