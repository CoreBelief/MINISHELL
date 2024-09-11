#ifndef ENVIRON_H
# define ENVIRON_H

# include "minishell.h"

// int		ft_init_env(char **envp);
int		init_env(t_shell *shell, char **envp);
char	*ft_get_env(const char *name, t_shell *shell);
int		ft_set_env(const char *name, const char *value, t_shell *shell);
int		ft_unset_env(const char *name, t_shell *shell);
void	ft_free_env(void);

#endif