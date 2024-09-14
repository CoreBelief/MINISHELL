#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "minishell.h"
#include "environ.h"
#include "signal.h"

#define MAX_HOSTNAME 256
#define MAX_USERNAME 256
#define MAX_PATH 1024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <limits.h>
#include <time.h>

//experimental page.... old version is commented out below since this time nonsense is illegal
//should proably pass the environment to this function to make it more legal


/* Function to get the current time in HH:MM:SS format */
char *get_current_time(void)
{
    time_t t;
    struct tm *timeinfo;
    char *time_str = malloc(9 * sizeof(char));

    if (!time_str)
        return (NULL);
    t = time(NULL);
    timeinfo = localtime(&t);
    strftime(time_str, 9, "%H:%M:%S", timeinfo);
    return (time_str);
}

/* Function to get the current working directory */
char *get_current_dir(void)
{
    char *cwd = getcwd(NULL, 0);
    if (!cwd)
        return (ft_strdup("unknown"));

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
            return (result);
        }
    }
    return (cwd);
}

/* Function to create and return the fancy prompt string */
char *create_prompt(void)
{
    char hostname[MAX_HOSTNAME];
    char username[MAX_USERNAME];
    char *cwd;
    char *prompt;
    char *time_str;
    size_t prompt_size;

    if (gethostname(hostname, sizeof(hostname)) != 0)
        ft_strcpy(hostname, "unknown");
    struct passwd *pw = getpwuid(getuid());
    if (pw)
        ft_strncpy(username, pw->pw_name, sizeof(username) - 1);
    else
        ft_strcpy(username, "user");
    cwd = get_current_dir();
    time_str = get_current_time();
    prompt_size = ft_strlen(username) + ft_strlen(hostname) + ft_strlen(cwd) + 100;
    prompt = malloc(prompt_size);
    if (prompt)
    {
        snprintf(prompt, prompt_size,
                 "\001\033[1;32m\002%s@\033[1;36m%s\001\033[0m\002:\033[1;33m%s\033[0m "
                 "\033[1;35m[%s]\033[0m ➜ ",
                 username, hostname, cwd, time_str);
    }
    free(cwd);
    free(time_str);

    return (prompt);
}

