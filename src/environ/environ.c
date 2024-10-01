/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   environ.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/28 12:25:53 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/30 19:31:35 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			init_env(t_shell *shell, char **envp);
char		*ft_get_env(const char *name, t_shell *shell);
static int	ft_add_env(char *new_var, t_shell *shell);
int			ft_set_env(const char *name, const char *value, t_shell *shell);
int			ft_unset_env(const char *name, t_shell *shell);

int	init_env(t_shell *shell, char **envp)
{
	int	i;

	shell->env_size = 0;
	while (envp[shell->env_size])
		shell->env_size++;
	shell->env = malloc(sizeof(char *) * (shell->env_size + 1));
	if (!shell->env)
	{
		perror("allocation failed for env");
		return (0);
	}
	i = 0;
	while (i < shell->env_size)
	{
		shell->env[i] = ft_strdup(envp[i]);
		if (!shell->env[i])
		{
			ft_free_str_array(shell->env);
			return (0);
		}
		i++;
	}
	shell->env[shell->env_size] = NULL;
	return (1);
}

char	*ft_get_env(const char *name, t_shell *shell)
{
	int		i;
	size_t	len;

	if (!name)
		return (NULL);
	i = 0;
	len = ft_strlen(name);
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0 && \
		shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static int	ft_add_env(char *new_var, t_shell *shell)
{
	int		i;
	char	**new_env;

	if (!new_var)
		return (0);
	new_env = (char **)malloc(sizeof(char *) * (shell->env_size + 2));
	if (!new_env)
	{
		free(new_var);
		return (0);
	}
	i = 0;
	while (shell->env && shell->env[i])
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	shell->env_size++;
	return (1);
}

int	ft_set_env(const char *name, const char *value, t_shell *shell)
{
	char	*new_var;
	int		i;

	if (!name || !value)
		return (0);
	new_var = ft_strjoin3(name, "=", value);
	if (!new_var)
		return (0);
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, ft_strlen(name)) == 0
			&& shell->env[i][ft_strlen(name)] == '=')
		{
			free(shell->env[i]);
			shell->env[i] = new_var;
			return (1);
		}
		i++;
	}
	return (ft_add_env(new_var, shell));
}

int	ft_unset_env(const char *name, t_shell *shell)
{
	int	i;
	int	j;

	if (!name)
		return (0);
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, ft_strlen(name)) == 0
			&& shell->env[i][ft_strlen(name)] == '=')
		{
			free(shell->env[i]);
			j = i;
			while (shell->env[j])
			{
				shell->env[j] = shell->env[j + 1];
				j++;
			}
			shell->env_size--;
			return (1);
		}
		i++;
	}
	return (0);
}
