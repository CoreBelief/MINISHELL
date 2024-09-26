/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/23 13:35:00 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/26 18:58:04 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "var_exp.h"

char	*var_exp_exit(int *i, t_shell *shell)
{
	char	*expansion;

	(*i) += 2;
	expansion = ft_itoa(shell->last_exit_status);
	return (expansion);
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

int	variable_exp_double(t_token *token, char *str, t_shell *shell)
{
	int		i;
	int		len;
	char	*expansion;
	char	*new_str;
	char	*tmp;
	char	*substr;

	i = 0;
	expansion = NULL;
	new_str = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				expansion = var_exp_exit(&i, shell);
				if (!expansion)
				{
					token->content = new_str;
					// free (new_str);
					return (1);
				}
				tmp = new_str;
				new_str = append_str(tmp, expansion);
				free(tmp);
				free(expansion);
				if (!new_str)
					return (0);
			}
			else if (is_var_char(str[i + 1]))
			{
				i++;
				expansion = variable_exp(str, &i, shell);
				if (!expansion && !str[i])
				{
					token->content = new_str;
					// free (new_str);
					return (1);
				}
				tmp = new_str;
				new_str = append_str(tmp, expansion);
				free(tmp);
				if (!new_str)
					return (0);
				// i += len;
			}
			else
			{
				tmp = new_str;
				substr = ft_strndup(&str[i], 1);
				if (!substr)
				{
					free (tmp);
					return (0);
				}
				new_str = append_str(tmp, substr);
				free (tmp);
				free (substr);
				if (!new_str)
					return (0);
				i++;
			}
		}
		len = until_dollar(&str[i]);
		if (len != 0)
		{
			tmp = new_str;
			substr = ft_strndup(&str[i], len);
			if (!substr)
			{
				free (tmp);
				return (0);
			}
			new_str = append_str(tmp, substr);
			free (tmp);
			free (substr);
			if (!new_str)
				return (0);
			// if (token->content)
				// free(token->content);
		}
		i += len;
	}
	token->content = new_str;
	return (1);
}
