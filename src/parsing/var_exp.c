/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/23 13:35:00 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/24 19:12:11 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "var_exp.h"

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

char	*append_str(char *og, char *to_append)
{
	char	*str;

	str = ft_strjoin(og, to_append);
	if (!str)
		return (NULL);
	return (str);
}

char	*var_exp_exit(int *i, t_shell *shell)
{
	char	*expansion;

	(*i) += 2;
	expansion = ft_itoa(shell->last_exit_status);
	return (expansion);
}

void	variable_exp_dollar(t_token *token, char *str, t_shell *shell)
{
	int		i;
	char	*expansion;
	char	*new_str;

	i = 0;
	expansion = NULL;
	new_str = ft_strdup("");
	//error handling
	if (str[i] == '$' && str[i + 1])
	{
		if (str[i + 1] == '?')
		{
			expansion = var_exp_exit(&i, shell);
		}
		else
		{
			i++;
			expansion = variable_exp(str, &i, shell);
			if (!expansion)
			{
				printf("expansion variable not found\n");
				// free (token->content);
				// token->content = "";
				// return ;
			}
		}
		new_str = append_str(new_str, expansion);
		new_str = append_str(new_str, &str[i]);
		token->content = new_str;
		// printf("token->cont %s\n", token->content);
		free (str);
	}
}

int	until_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	return (i);
}


void	variable_exp_double(t_token *token, char *str, t_shell *shell)
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
	//error handling
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				expansion = var_exp_exit(&i, shell);
				if (!expansion)
				{
					free (new_str);
					return ;
				}
				tmp = new_str;
				new_str = append_str(tmp, expansion);
				free(tmp);
				if (!new_str)
					return ;
			}
			else if (is_var_char(str[i + 1]))
			{
				i++;
				expansion = variable_exp(str, &i, shell);
				if (!expansion)
				{
					free (new_str);
					return ;
				}
				tmp = new_str;
				new_str = append_str(tmp, expansion);
				free(tmp);
				if (!new_str)
					return ;
				// i += len;
			}
			else
			{
				tmp = new_str;
				substr = ft_strndup(&str[i], 1);
				if (!substr)
				{
					free (tmp);
					return ;
				}
				new_str = append_str(tmp, substr);
				free (tmp);
				free (substr);
				if (!new_str)
					return ;
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
				return ;
			}
			new_str = append_str(tmp, substr);
			free (tmp);
			free (substr);
			if (!new_str)
				return ;
		}
		i += len;
		token->content = new_str;
	}
}

