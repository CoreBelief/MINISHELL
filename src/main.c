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
void minishell_loop(t_shell *shell);
void	free_shell(t_shell *shell); //move this to memory.c ?
void	init_shell(t_shell *shell); //move this also somewhere else?
void ft_custom_itoa(int n, char *str, int max_len); //this one can use its own file inside utils maybe?
void increment_shlvl(t_shell *shell); //not sure yet where to place this?
int	main(int argc, char **argv, char **envp); //main can be shorter and split up maybe? maybe its fine


static void	process_input(char *line, t_shell *shell)
{
	t_token		*tokens;

	if (line && *line)
	{
		add_history(line);
		tokens = tokenizer(line, shell);
		if (!tokens)
		{
			free_tokens(&tokens);
			shell->last_exit_status = 1;
			return ;
		}
		shell->commands = parse_command_from_tokens(tokens, shell);
		if (!shell->commands)
		{
			shell->last_exit_status = 1;
			free_tokens(&tokens);
			free_command_list(&shell->commands);
			return ;
		}
		free_tokens(&tokens);
		execute_command(shell);
		free_command_list(&shell->commands);
	}
}

void minishell_loop(t_shell *shell)
{ //function too long needs splitting up!
    char *line = NULL;
    char *prompt = NULL;
    int original_stdout;
    int terminal_fd;

    setup_signals_shell();
    original_stdout = dup(STDOUT_FILENO);
    terminal_fd = open("/dev/tty", O_WRONLY);
    if (terminal_fd == -1)
    {
        perror("Failed to open terminal");
        return;
    }
    while (1)
    {
        dup2(terminal_fd, STDOUT_FILENO);
        if (isatty(STDIN_FILENO))
        {
            prompt = create_prompt();
            if (!prompt)
            {
                ft_putstr_fd("Error: Failed to create prompt\n", terminal_fd);
                shell->last_exit_status = 1;
                break;
            }
            
            line = readline(prompt);
            free(prompt);
            if (!line) 
            {
                ft_putstr_fd("exit\n", terminal_fd);
                break;
            }
            add_history(line);
        }
        else
        {
            line = get_next_line(STDIN_FILENO);
            if (!line)
                break;
            size_t len = ft_strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';
        }
        dup2(original_stdout, STDOUT_FILENO);
        process_input(line, shell);
        free(line);
        line = NULL;
    }
    if (line)
        free(line);
    close(terminal_fd);
    close(original_stdout);
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

void ft_custom_itoa(int n, char *str, int max_len)
{ //needs to be norm proof... maybe we can use regualr itoa??
    int i = 0;
    int is_negative = 0;
    
    if (n == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (n < 0)
    {
        is_negative = 1;
        n = -n;
    }
    while (n != 0 && i < max_len - 1)
    {
        str[i++] = (n % 10) + '0';
        n = n / 10;
    }
    if (is_negative && i < max_len - 1)
        str[i++] = '-';
    str[i] = '\0';
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void increment_shlvl(t_shell *shell)
{
    char *shlvl_str = ft_get_env("SHLVL", shell);
    int shlvl = 1;
    if (shlvl_str != NULL)
    {
        shlvl = ft_atoi(shlvl_str);
        shlvl += 1;
    }
    char new_shlvl[16];
    ft_custom_itoa(shlvl, new_shlvl, sizeof(new_shlvl));//find out why custom itoa and why not regular ft_itoa
    ft_set_env("SHLVL", new_shlvl, shell);
}

int	main(int argc, char **argv, char **envp)
{//how to save 1 line??
	t_shell	shell;
	
	init_shell(&shell);
	if (!init_env(&shell, envp))
	{
		perror("error init env");
		return (EXIT_FAILURE);
	}
    increment_shlvl(&shell);
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

