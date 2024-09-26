/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 12:36:10 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/26 19:49:56 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	find_var_len(char *var)
{
	int	i;

	i = 0;
	while (var[i] && is_var_char(var[i]))
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
		i++;
	return (i);
}

char	*variable_exp(char *str, int *i, t_shell *shell)
{
	int		len;
	char	*expansion;
	char	*var;

	len = find_var_len(&str[*i]);
	var = ft_strndup(&str[*i], len);
	if (!var)
		return (NULL);
	expansion = ft_get_env(var, shell);
	free (var);
	*i += len;
	return (expansion);
}
