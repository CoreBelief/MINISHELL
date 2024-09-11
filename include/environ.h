#ifndef ENVIRON_H
# define ENVIRON_H

# include "minishell.h"
# include "structs.h"
int		init_env(t_shell *shell, char **envp);

char	*ft_get_env(const char *name, t_shell *shell);

int		ft_set_env(const char *name, const char *value, t_shell *shell);

int		ft_unset_env(const char *name, t_shell *shell);

#endif