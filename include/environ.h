#ifndef ENVIRON_H
# define ENVIRON_H

int		ft_init_env(char **envp);
char	*ft_get_env(const char *name);
int		ft_set_env(const char *name, const char *value);
int		ft_unset_env(const char *name);
void	ft_free_env(void);

#endif