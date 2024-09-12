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

#define MAX_HOSTNAME 256
#define MAX_USERNAME 256
#define MAX_PATH 1024


int g_exit_status = 0;

void update_exit_status(int status)
{
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        g_exit_status = 128 + WTERMSIG(status);
}

static char *get_current_dir(void)
{
    char *cwd = getcwd(NULL, 0);
    if (!cwd)
        return ft_strdup("unknown");

    char *home = getenv("HOME");
    if (home && ft_strncmp(cwd, home, ft_strlen(home)) == 0)
    {
        char *relative_path = cwd + ft_strlen(home);
        char *result = malloc(ft_strlen(relative_path) + 2);
        if (result)
        {
            result[0] = '~';
            ft_strcpy(result + 1, relative_path);
            free(cwd);
            return result;
        }
    }
    return cwd;
}

static char *create_prompt(void)
{
    char hostname[MAX_HOSTNAME];
    char username[MAX_USERNAME];
    char *cwd;
    char *prompt;

    if (gethostname(hostname, sizeof(hostname)) != 0)
        ft_strcpy(hostname, "unknown");
    
    struct passwd *pw = getpwuid(getuid());
    if (pw)
        ft_strncpy(username, pw->pw_name, sizeof(username) - 1);
    else
        ft_strcpy(username, "user");

    cwd = get_current_dir();

    size_t prompt_size = ft_strlen(username) + ft_strlen(hostname) + ft_strlen(cwd) + 50;
    prompt = malloc(prompt_size);
    if (prompt)
snprintf(prompt, prompt_size, "\001\033[1;35m\002%s@%s\001\033[0m\002:\001\033[1;33m\002%s\001\033[0m\002$ ", username, hostname, cwd);

    free(cwd);
    return prompt;
}

static void	process_input(char *line, t_shell *shell)
{
	t_token		*tokens;

	if (line && *line)
	{
		add_history(line);
		tokens = tokenizer(line);
		if (!tokens)
			return ;
		shell->commands = parse_command_from_tokens(tokens, shell);
		if (!shell->commands)
		{
			free_tokens(&tokens);
			return ;
		}
		free_tokens(&tokens);
		// print_cmd_list(shell->commands);
		execute_command(shell);
        // Handle any pending signals
        handle_pending_signals();
        free_command_list(&shell->commands);
	}
}

// char	*get_full_line(char *line)
// {
// 	// char	*line;
// 	char	*next_line;
// 	char	*temp;

// 	// line = readline(prompt);
// 	if (line[ft_strlen(line) - 1] == '|')
// 	{
// 		next_line = readline("> ");
// 		if (!next_line)
// 		{
// 			free(line);
// 			return (NULL);
// 		}
// 		temp = line;
// 		line = ft_strjoin(line, next_line);
// 		free(temp);
// 		free(next_line);
// 	}
// 	return (line);

// }

void	minishell_loop(t_shell *shell)
{
	char	*line;
	char	*prompt;
	// rl_catch_signals = 0; // Prevent readline from handling signals 

	setup_signals_shell();
	while (1)
	{
		prompt = create_prompt();
		if (!prompt)
		{
			fprintf(stderr, "Error: Failed to create prompt\n"); //cannot use fprintf
			break ;
		}
		line = readline(prompt);
		// line = get_full_line(line);
		free(prompt);
		handle_pending_signals();
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		int sig = get_and_reset_signal();
		if (sig == SIGINT)
		{
			shell->last_exit_status = 130; // Set exit status to 130 for exit command
			free(line);
			// Handle interrupt (e.g., clear the line, continue the loop)
			continue;
		}
		process_input(line, shell);
		free(line);
		// reset_signals(); // Reset signals after each command
	}
}
void	init_shell(t_shell *shell)
{
	shell->commands = NULL;
	shell->last_exit_status = 0;
	shell->env = NULL;
	shell->env_size = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell);
	if (!init_env(&shell, envp))
	{
		perror("error init env");
		return (1);
	}
	minishell_loop(&shell);
	return (shell.last_exit_status);
}

