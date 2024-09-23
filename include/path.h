#ifndef PATH_H
# define PATH_H

#include "minishell.h"

char	*search_paths(char **paths, char *command);

char	*find_executable(char *command, t_shell *shell);

char	*find_command_in_path(char *command, t_shell *shell);

int		check_permissions(char *path, t_shell *shell);

#endif