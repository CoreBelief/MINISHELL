#include "builtins.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>

void	builtin_unset(char **args, t_shell *shell);
static int	is_valid_first_char(char c); 
static int	is_valid_variable_char(char c);
static int	is_valid_variable_name(const char *name);

static int	is_valid_first_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static int	is_valid_variable_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	is_valid_variable_name(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!is_valid_first_char(name[0]))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!is_valid_variable_char(name[i]))
			return (0);
		i++;
	}
	return (1);
}

void builtin_unset(char **args, t_shell *shell)
{
    int i = 1;

    if (!args[1])
        return;

    while (args[i])
    {
        if (is_valid_variable_name(args[i]))
            ft_unset_env(args[i], shell);
        i++;
    }
}