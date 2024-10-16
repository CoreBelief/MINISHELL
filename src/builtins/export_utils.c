/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/16 17:26:26 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/16 17:28:34 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_to_export_list(char *var, t_shell *shell);
static int	set_environment(char *arg, char *equal_sign, t_shell *shell);
static int	process_environment(char *name, char *value, int append_mode,
				t_shell *shell);
static int	find_export_index(char *name, t_shell *shell);
static char	*create_new_value(char *name, char *value, int append_mode,
				t_shell *shell);
static int	update_environment(char *name, char *new_value, int index,
				t_shell *shell);
static void	restore_equal_sign(char *equal_sign, int append_mode);
int			process_identifier(char *arg, char *equal_sign, t_shell *shell);

static int	add_to_export_list(char *var, t_shell *shell)
{
	char	**new_list;
	int		i;

	new_list = malloc(sizeof(char *) * (shell->export_size + 2));
	if (!new_list)
		return (0);
	i = -1;
	while (++i < shell->export_size)
		new_list[i] = shell->export_list[i];
	new_list[i] = ft_strdup(var);
	new_list[i + 1] = NULL;
	free(shell->export_list);
	shell->export_list = new_list;
	shell->export_size++;
	return (1);
}

static int	set_environment(char *arg, char *equal_sign, t_shell *shell)
{
	char	*name;
	char	*value;
	int		append_mode;

	name = arg;
	value = NULL;
	append_mode = 0;
	if (equal_sign)
	{
		*equal_sign = '\0';
		value = equal_sign + 1;
		if (equal_sign > arg && *(equal_sign - 1) == '+')
		{
			append_mode = 1;
			*(equal_sign - 1) = '\0';
		}
	}
	if (!process_environment(name, value, append_mode, shell))
		return (0);
	restore_equal_sign(equal_sign, append_mode);
	return (1);
}

static int	process_environment(char *name, char *value, int append_mode,
		t_shell *shell)
{
	int		index;
	char	*new_value;

	index = find_export_index(name, shell);
	if (value)
	{
		new_value = create_new_value(name, value, append_mode, shell);
		if (!new_value || !update_environment(name, new_value, index, shell))
		{
			free(new_value);
			return (0);
		}
		free(new_value);
	}
	else if (index == shell->export_size)
	{
		if (!add_to_export_list(name, shell))
			return (0);
	}
	return (1);
}

static int	find_export_index(char *name, t_shell *shell)
{
	int	i;
	int	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (i < shell->export_size)
	{
		if (ft_strncmp(shell->export_list[i], name, name_len) == 0
			&& (shell->export_list[i][name_len] == '='
			|| shell->export_list[i][name_len] == '\0'))
			return (i);
		i++;
	}
	return (i);
}

static char	*create_new_value(char *name, char *value, int append_mode,
		t_shell *shell)
{
	char	*old_value;
	char	*new_value;

	old_value = ft_get_env(name, shell);
	if (append_mode && old_value)
		new_value = ft_strjoin(old_value, value);
	else
		new_value = ft_strdup(value);
	return (new_value);
}

static int	update_environment(char *name, char *new_value, int index,
		t_shell *shell)
{
	char	*temp;

	if (!ft_set_env(name, new_value, shell))
		return (0);
	if (index < shell->export_size)
		free(shell->export_list[index]);
	else
	{
		shell->export_size++;
		shell->export_list = realloc(shell->export_list, sizeof(char *)
				* (shell->export_size + 1));
		if (!shell->export_list)
			return (0);
		shell->export_list[shell->export_size] = NULL;
	}
	shell->export_list[index] = ft_strjoin(name, "=");
	temp = ft_strjoin(shell->export_list[index], new_value);
	free(shell->export_list[index]);
	shell->export_list[index] = temp;
	return (1);
}

static void	restore_equal_sign(char *equal_sign, int append_mode)
{
	if (equal_sign)
	{
		*equal_sign = '=';
		if (append_mode)
			*(equal_sign - 1) = '+';
	}
}

// static int set_environment(char *arg, char *equal_sign, t_shell *shell)
// {
//     char *name, *value;
//     int i, append_mode = 0;

//     name = arg;
//     if (equal_sign)
//     {
//         *equal_sign = '\0';
//         value = equal_sign + 1;
//         if (equal_sign > arg && *(equal_sign - 1) == '+')
//         {
//             append_mode = 1;
//             *(equal_sign - 1) = '\0';
//         }
//     }
//     else
//         value = NULL;
//     for (i = 0; i < shell->export_size; i++)
//     {
//         if (ft_strncmp(shell->export_list[i], name, ft_strlen(name)) == 0
//             && (shell->export_list[i][ft_strlen(name)] == '='
//                 || shell->export_list[i][ft_strlen(name)] == '\0'))
//         {
//             break ;
//         }
//     }
//     if (value != NULL)
//     {
//         char *old_value = ft_get_env(name, shell);
//         char *new_value;

//         if (append_mode && old_value)
//         {
//             new_value = ft_strjoin(old_value, value);
//         }
//         else
//         {
//             new_value = ft_strdup(value);
//         }

//         if (!ft_set_env(name, new_value, shell))
//         {
//             free(new_value);
//             return (0);
//         }
//         if (i < shell->export_size)
//         {
//             free(shell->export_list[i]);
//         }
//         else
//         {
//             shell->export_size++;
//             shell->export_list = realloc(shell->export_list, sizeof(char *)
		// * (shell->export_size + 1));
//             shell->export_list[shell->export_size] = NULL;
//         }

//         shell->export_list[i] = ft_strjoin(name, "=");
//         char *temp = ft_strjoin(shell->export_list[i], new_value);
//         free(shell->export_list[i]);
//         shell->export_list[i] = temp;

//         free(new_value);
//     }
//     else if (i == shell->export_size)
//     {
//         if (!add_to_export_list(name, shell))
//             return (0);
//     }
//     if (equal_sign)
//     {
//         *equal_sign = '=';
//         if (append_mode)
//             *(equal_sign - 1) = '+';
//     }
//     return (1);
// }

int	process_identifier(char *arg, char *equal_sign, t_shell *shell)
{
	char	*identifier;
	int		result;

	if (equal_sign != NULL)
	{
		if (!handle_equal_sign(arg, equal_sign, &identifier))
		{
			shell->last_exit_status = 1;
			return (0);
		}
	}
	else
		handle_no_equal_sign(arg, &identifier);
	if (!validate_identifier(identifier, arg, shell))
	{
		if (equal_sign != NULL)
			free(identifier);
		return (0);
	}
	result = set_environment(arg, equal_sign, shell);
	if (!result)
		shell->last_exit_status = 1;
	if (equal_sign != NULL)
		free(identifier);
	return (result);
}
