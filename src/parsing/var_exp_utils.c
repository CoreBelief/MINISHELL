/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 12:36:10 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/04 16:04:17 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		handle_variable(char **new_str, char *str, int *i, t_shell *shell);
int		find_var_len(char *var);
char	*append_str(char *og, char *to_append);
int		until_dollar(char *str);
char	*variable_exp(char *str, int *i, t_shell *shell);

int	handle_variable(char **new_str, char *str, int *i, t_shell *shell)
{
	char	*expansion;
	char	*tmp;

	(*i)++;
	expansion = variable_exp(str, i, shell);
	if (!expansion)
		return (0);
	tmp = *new_str;
	*new_str = append_str(tmp, expansion);
	free(expansion);
	free(tmp);
	if (!new_str)
		return (0); // malloc
	return (1);
}

int	find_var_len(char *var)
{
	int	i;

	i = 0;
	while (var[i] && (ft_isalnum(var[i]) || var[i] == '_'))
		i++;
	return (i);
}

char	*append_str(char *og, char *to_append)
{
	char	*str;

	if (!to_append)
		return (ft_strdup(og));
	str = ft_strjoin(og, to_append);
	if (!str)
		return (NULL);
	return (str);
}

int	until_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '$')
	{
		if (str[i] == '\\' && str[i + 1] && str[i + 1] == '$')
			i += 2;
		else if (str[i] == '$')
			return (i);
		else
			i++;
	}
	return (i);
}

char	*variable_exp(char *str, int *i, t_shell *shell)
{
	int		len;
	char	*expansion;
	char	*var;
	char	*env_value;

	len = find_var_len(&str[*i]);
	var = ft_strndup(&str[*i], len);
	if (!var)
		return (NULL); // malloc
	env_value = ft_get_env(var, shell);
	if (!env_value)
		expansion = ft_strdup("");
	else
		expansion = ft_strdup(env_value);
	free(var);
	*i += len;
	return (expansion); // malloc fail if return NULL
}
