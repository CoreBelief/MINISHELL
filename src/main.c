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

static void	process_input(char *line)
{
	t_token		*tokens;
	t_command	*cmd;

	if (line && *line)
	{
		add_history(line);
		tokens = tokenizer(line);
		if (!tokens)
			return ;
		cmd = parse_command_from_tokens(tokens);
		if (!cmd)
		{
			free_tokens(&tokens);
			return ;
		}
		free_tokens(&tokens);
		execute_command(cmd);
        free_command_list(&cmd);
		update_exit_status(g_exit_status);
	}
}

void minishell_loop(void)
{
    char *line;
    char *prompt;

    setup_signals_shell();

    while (1)
    {
        prompt = create_prompt();
        if (!prompt)
        {
            fprintf(stderr, "Error: Failed to create prompt\n");
            break;
        }
        line = readline(prompt);
        free(prompt);
        
        if (!line)
        {
            printf("exit\n");
            break;
        }
        
        if (ft_strcmp(line, "exit") == 0)
        {
            free(line);
            break;
        }
        
        process_input(line);
        free(line);
        reset_signals(); // Reset signals after each command
    }
}


int main(void)
{
    minishell_loop();
    return (g_exit_status);
}

