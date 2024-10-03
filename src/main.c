#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "environ.h"
#include "signal.h"
#include <fcntl.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

static void	process_input(char *line, t_shell *shell); //maybe we move this to different file?
void		minishell_loop(t_shell *shell);
void		free_shell(t_shell *shell); //move this to memory.c ?
void		init_shell(t_shell *shell); //move this also somewhere else?
void		ft_custom_itoa(int n, char *str, int max_len); //this one can use its own file inside utils maybe?
int			increment_shlvl(t_shell *shell); //not sure yet where to place this?
int			main(int argc, char **argv, char **envp); //main can be shorter and split up maybe? maybe its fine


static void	process_input(char *line, t_shell *shell)
{
	t_token		*tokens;

	if (line && *line)
	{
		add_history(line);
		tokens = tokenizer(line, shell);
		if (!tokens)
			return ;
		// print_token_list(tokens);
		if (!parse_command_from_tokens(tokens, shell))
		{
			// shell->last_exit_status = 1;
			free_tokens(&tokens);
			return ;
		}
		// print_cmd_list(shell->commands);
		free_tokens(&tokens);
		execute_command(shell);
		free_command_list(&shell->commands);
	}
}
// void minishell_loop(t_shell *shell)
// {
//     char *line = NULL;
//     char *prompt = NULL;
//     int original_stdout;
//     int terminal_fd;

//     setup_signals_shell();
//     original_stdout = dup(STDOUT_FILENO); // Keep this, but it may still be problematic.
//     terminal_fd = open("/dev/tty", O_WRONLY);
//     if (terminal_fd == -1)
//     {
//         perror("Failed to open terminal");
//         return;
//     }
//     while (1)
//     {
//         // dup2(terminal_fd, STDOUT_FILENO);

//         // Commented out isatty checks to force always interactive behavior
//         // if (isatty(STDIN_FILENO)) 
//         // {
//             prompt = create_prompt();
//             if (!prompt)
//             {
//                 ft_putstr_fd("Error: Failed to create prompt\n", terminal_fd);
//                 shell->last_exit_status = 1;
//                 break;
//             }

//             line = readline(prompt);
//             free(prompt);
//             if (!line) 
//             {
//                 // ft_putstr_fd("exit\n", terminal_fd);
//                 break;
//             }
//             add_history(line);
//         // } 
//         // else 
//         // {
//         //     line = get_next_line(STDIN_FILENO);
//         //     if (!line)
//         //         break;
//         //     size_t len = ft_strlen(line);
//         //     if (len > 0 && line[len - 1] == '\n')
//         //         line[len - 1] = '\0';
//         // }

//         // dup2(original_stdout, STDOUT_FILENO);
//         process_input(line, shell);
//         free(line);
//         line = NULL;
//     }
//     if (line)
//         free(line);
//     close(terminal_fd);
//     close(original_stdout);
// }
void minishell_loop(t_shell *shell)
{
    char *line = NULL;
    char *prompt = NULL;

    setup_signals_shell();

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            // Interactive mode
            prompt = create_prompt();
            if (!prompt)
            {
                perror("Error: Failed to create prompt");
                shell->last_exit_status = 1;
                break;
            }
            line = readline(prompt);
            free(prompt);

            if (!line)
            {
                printf("exit\n");
                break;
            }
            if (*line)
                add_history(line);
        }
        else
        {
            // Non-interactive mode
            line = get_next_line(STDIN_FILENO);
            if (!line)
                break;

            // Remove newline character if present
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';
        }

        if (*line)
            process_input(line, shell);

        free(line);
        line = NULL;
    }
}


void	free_shell(t_shell *shell)
{
	if (shell->env)
	{
		ft_free_str_array(shell->env);
	}
}

void	init_shell(t_shell *shell)
{
	shell->commands = NULL;
	shell->last_exit_status = 0;
	shell->env = NULL;
	shell->env_size = 0;
}

static int	countnum(long n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*intstring;
	int		len;
	long	num;
	int		i;

	num = (long)n;
	len = countnum(num);
	intstring = (char *) malloc ((len + 1) * (sizeof(char)));
	if (!intstring)
		return (NULL);
	i = 0;
	if (num < 0)
		num = -num;
	intstring[len] = '\0';
	while (i < len)
	{
		intstring[len - 1 - i] = (num % 10) + '0';
		num = num / 10;
		i++;
	}
	if (n < 0)
		intstring[0] = '-';
	return (intstring);
}

int	increment_shlvl(t_shell *shell)
{
	char	*shlvl_str;
	char	*new_shlvl;
	int		shlvl;

	shlvl = 1;
	shlvl_str = ft_get_env("SHLVL", shell);
	if (shlvl_str != NULL)
	{
		shlvl = ft_atoi(shlvl_str);
		shlvl += 1;
	}
	new_shlvl = ft_itoa(shlvl);
	// ft_custom_itoa(shlvl, new_shlvl, sizeof(new_shlvl));//find out why custom itoa and why not regular ft_itoa
	if (!ft_set_env("SHLVL", new_shlvl, shell))
	{
		free (new_shlvl);
		return (0);
	}
	free (new_shlvl);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	init_shell(&shell);
	if (!init_env(&shell, envp))
		return (perror("error init env"), EXIT_FAILURE);
	if (!increment_shlvl(&shell))
		return (EXIT_FAILURE);
	if (argc > 1 && strcmp(argv[1], "-c") == 0)
	{
		if (argc > 2)
		{
			process_input(argv[2], &shell);
			free_shell(&shell);
			return (shell.last_exit_status);
		}
		else
		{
			ft_putendl_fd("Error: -c requires an argument", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	minishell_loop(&shell);
	free_shell(&shell);
	return (shell.last_exit_status);
}

