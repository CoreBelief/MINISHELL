/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rdl <rdl@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 17:29:41 by rdl           #+#    #+#                 */
/*   Updated: 2024/09/26 17:29:43 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool	is_only_n_flag(char *arg);
void 		builtin_echo(char **args);
// make norm proof!!!
static char* remove_paired_quotes(const char* input) {
    int len = strlen(input);
    char* output = malloc(len + 1);  // Allocate memory for the worst case
    int i = 0, j = 0;
    
    while (i < len) {
        if (input[i] == '\'' || input[i] == '\"') {
            char quote = input[i];
            int end = i + 1;
            while (end < len && input[end] != quote) {
                end++;
            }
            if (end < len) {  // Matching quote found
                // Copy content between quotes
                memcpy(output + j, input + i + 1, end - i - 1);
                j += end - i - 1;
                i = end + 1;
            } else {
                // No matching quote, treat as normal character
                output[j++] = input[i++];
            }
        } else {
            output[j++] = input[i++];
        }
    }
    
    output[j] = '\0';  // Null-terminate the string
    return output;
}

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

void builtin_echo(char **args)
{
    int i = 1;
    bool newline = true;
    if (!args)
        return;
    while (args[i] && is_only_n_flag(args[i]))
    {   
        newline = false;
        i++;
    }
    while (args[i])
    {   
        char *processed = remove_paired_quotes(args[i]);
        printf("%s", processed);
        free(processed);  // Don't forget to free the allocated memory
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
}
