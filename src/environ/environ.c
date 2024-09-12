#include "environ.h"
#include "utils.h"
#include <stdlib.h>
#include "minishell.h"


int			init_env(t_shell *shell, char **envp);
char		*ft_get_env(const char *name, t_shell *shell);
static int 	ft_add_env(char *new_var, t_shell *shell);
int			ft_set_env(const char *name, const char *value, t_shell *shell);
int			ft_unset_env(const char *name, t_shell *shell);

int	init_env(t_shell *shell, char **envp)
{
	int	i;

	i = 0;
	while (envp[shell->env_size])
		shell->env_size++;
	shell->env = malloc(sizeof(char *) * (shell->env_size + 1));
	if (!shell->env)
	{
		perror("allocation failed for env");
		return (0);
	}
	while (envp[i])
	{
		shell->env[i] = ft_strdup(envp[i]);
		if (!shell->env[i])
		{
			// Clean up on failure
			while (--i >= 0)
				free(shell->env[i]);
			free(shell->env);
			return (0);
		}
		i++;
	}
	shell->env[shell->env_size] = NULL;
	return (1);
}

char	*ft_get_env(const char *name, t_shell *shell)
{
	int	i;
	size_t	len;

	if (!name) // Check if name is NULL
		return (NULL);
	i = 0;
	len = ft_strlen(name);
	while (shell->env && shell->env[i]) // Ensure shell->env is not NULL
	{
		if (ft_strncmp(shell->env[i], name, len) == 0 && shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static int ft_add_env(char *new_var, t_shell *shell)
{
	int i;
	char **new_env;

	if (!new_var) // Check if new_var is NULL
		return (0);

	// i = 0;
	// while (environ && environ[i])
	// 	i++;
	new_env = (char **)malloc(sizeof(char *) * (shell->env_size + 2));
	if (!new_env)
		return (0);
	i = 0;
	while (shell->env && shell->env[i])
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(shell->env); // Free the old shell->envment
	shell->env = new_env;
	shell->env_size++;
	return (1);
}

int ft_set_env(const char *name, const char *value, t_shell *shell)
{
	char *new_var;
	int i;

	if (!name || !value) // Check if name or value is NULL
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
			free(shell->env[i]); // Free the old variable
			shell->env[i] = new_var;
			return (1);
		}
		i++;
	}
	return (ft_add_env(new_var, shell));
}

int ft_unset_env(const char *name, t_shell *shell)
{
	int i;
	int j;

	if (!name) // Check if name is NULL
		return (0);

	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, ft_strlen(name)) == 0
			&& shell->env[i][ft_strlen(name)] == '=')
		{
			free(shell->env[i]); // Free the variable to be unset
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
