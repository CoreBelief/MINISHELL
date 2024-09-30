/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/23 13:35:00 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/30 16:22:14 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "var_exp.h"
int	handle_exit_status(char **new_str, int *i, t_shell *shell);
int	handle_variable(char **new_str, char *str, int *i, t_shell *shell);
int	handle_dollar(char **new_str, char *str, int *i, t_shell *shell);
int	handle_non_dollar(char **new_str, char *str, int *i);
char	*variable_exp_double(char *str, t_shell *shell);



int	handle_exit_status(char **new_str, int *i, t_shell *shell)
{
	char	*expansion;
	char	*tmp;

	(*i) += 2;
	expansion = ft_itoa(shell->last_exit_status);
	if (!expansion)
	{
		free(*new_str);
		return (0);
	}
	tmp = *new_str;
	*new_str = append_str(tmp, expansion);
	free(tmp);
	free(expansion);
	if (!*new_str)
		return (0);
	return (1);
}

int	handle_variable(char **new_str, char *str, int *i, t_shell *shell)
{
	char	*expansion;
	char	*tmp;

	(*i)++;
	expansion = variable_exp(str, i, shell);
	if (!expansion)
	{
		// token->content = new_str;
		return (1);
	}
	tmp = *new_str;
	*new_str = append_str(tmp, expansion);
	free(tmp);
	if (!new_str)
		return (0);
	return (1);
}

int	handle_dollar(char **new_str, char *str, int *i, t_shell *shell)
{
	char	*tmp;
	char	*substr;

	if (str[*i + 1] == '?')
		return (handle_exit_status(new_str, i, shell));
	else if (is_var_char(str[*i + 1]))
		return (handle_variable(new_str, str, i, shell));
	else
	{
		tmp = *new_str;
		substr = ft_strndup(&str[*i], 1);
		if (!substr)
		{
			free (tmp);
			return (0);
		}
		*new_str = append_str(tmp, substr);
		free (tmp);
		free (substr);
		if (!*new_str)
			return (0);
		(*i)++;
	}
	return (1);
}

int	handle_non_dollar(char **new_str, char *str, int *i)
{
	int		len;
	char	*tmp;
	char	*substr;

	len = until_dollar(&str[*i]);
	if (len != 0)
	{
		tmp = *new_str;
		substr = ft_strndup(&str[*i], len);
		if (!substr)
		{
			free (tmp);
			return (0);
		}
		*new_str = append_str(tmp, substr);
		free (tmp);
		free (substr);
		if (!*new_str)
			return (0);
		// if (token->content)
			// free(token->content);
	}
	*i += len;
	return (1);
}

char	*variable_exp_double(char *str, t_shell *shell)
{
	int		i;
	char	*new_str;
	char	*tmp;

	i = 0;
	new_str = ft_strdup("");
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			tmp = new_str;
			if (!handle_dollar(&new_str, str, &i, shell))
				return (0);
			if (tmp == new_str)
				return (new_str);
		}
		else
		{
			if (!handle_non_dollar(&new_str, str, &i))
				return (NULL);
		}
	}
	return (new_str);
}
