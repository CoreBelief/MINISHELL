/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:19:15 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 18:42:45 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H

# include "minishell.h"

char	*search_paths(char **paths, char *command);
char	*find_executable(char *command, t_shell *shell);
void	check_file_status(char *path, t_shell *shell);
void	cleanup_heredoc_files(t_cmd *cmd);

#endif