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
        return strdup("unknown");

    char *home = getenv("HOME");
    if (home && strncmp(cwd, home, strlen(home)) == 0)
    {
        char *relative_path = cwd + strlen(home);
        char *result = malloc(strlen(relative_path) + 2);
        if (result)
        {
            result[0] = '~';
            strcpy(result + 1, relative_path);
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
        strcpy(hostname, "unknown");
    
    struct passwd *pw = getpwuid(getuid());
    if (pw)
        strncpy(username, pw->pw_name, sizeof(username) - 1);
    else
        strcpy(username, "user");

    cwd = get_current_dir();

    size_t prompt_size = strlen(username) + strlen(hostname) + strlen(cwd) + 50;
    prompt = malloc(prompt_size);
    if (prompt)
        snprintf(prompt, prompt_size, "\001\033[1;32m\002%s@%s\001\033[0m\002:\001\033[1;34m\002%s\001\033[0m\002$ ", username, hostname, cwd);

    free(cwd);
    return prompt;
}

static void process_input(char *line)
{
    if (line && *line)
    {
        add_history(line);
        execute_command(line);
        update_exit_status(g_exit_status);
    }
}

void minishell_loop(void)
{
    char *line;
    char *prompt;

    setup_signals();
    while (1)
    {
        prompt = create_prompt();
        line = readline(prompt);
        free(prompt);

        if (!line)
        {
            printf("exit\n");
            break;
        }
        process_input(line);
        free(line);
    }
}

int main(void)
{
    minishell_loop();
    return (g_exit_status);
}