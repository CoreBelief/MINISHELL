
#include "builtins.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//too many functions in file!!
// static char	*remove_paired_quotes(const char *input);
static bool	is_only_n_flag(char *arg);
void		builtin_echo(char **args);
// static char	*allocate_output(const char *input);
// static int	find_matching_quote(const char *input, int start, int len, char q);
// static void	copy_between_quotes(char *output, const char *input, int s, int e);
// static char	*process_input(const char *input, char *output);

// static char	*allocate_output(const char *input)
// {
// 	int	len;

// 	len = ft_strlen(input);
// 	return (malloc(len + 1));
// }

// static int	find_matching_quote(const char *input, int start, int len, char q)
// {
// 	int	end;

// 	end = start + 1;
// 	while (end < len && input[end] != q)
// 		end++;
// 	return (end);
// }

// static void	copy_between_quotes(char *output, const char *input, int s, int e)
// {
// 	ft_memcpy(output, input + s + 1, e - s - 1);
// }

// static char	*process_input(const char *input, char *output)
// {
// 	int		i;
// 	int		j;
// 	int		len;
// 	int		end;
// 	char	quote;

// 	i = 0;
// 	j = 0;
// 	len = ft_strlen(input);
// 	while (i < len)
// 	{
// 		if (input[i] == '\'' || input[i] == '\"')
// 		{
// 			quote = input[i];
// 			end = find_matching_quote(input, i, len, quote);
// 			if (end < len)
// 			{
// 				copy_between_quotes(output + j, input, i, end);
// 				j += end - i - 1;
// 				i = end + 1;
// 			}
// 			else
// 				output[j++] = input[i++];
// 		}
// 		else
// 			output[j++] = input[i++];
// 	}
// 	output[j] = '\0';
// 	return (output);
// }

// static char	*remove_paired_quotes(const char *input)
// {
// 	char	*output;

// 	output = allocate_output(input);
// 	if (!output)
// 		return (NULL);
// 	return (process_input(input, output));
// }

static bool	is_only_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

void	builtin_echo(char **args)
{
	int		i;
	bool	newline;
	// char	*processed;

	i = 1;
	newline = true;
	if (!args)
		return ;
	while (args[i] && is_only_n_flag(args[i]))
	{
		newline = false;
		i++;
	}
	while (args[i])
	{
		// processed = remove_paired_quotes(args[i]);
		printf("%s", args[i]);
		// free(processed);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}