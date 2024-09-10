#include "environ.h"
#include "utils.h"
#include <stdlib.h>
#include "minishell.h"

static char **g_env = NULL;

int ft_init_env(char **envp)
{
    int i;
    int size;

    size = 0;
    while (envp[size])
        size++;
    g_env = (char **)malloc(sizeof(char *) * (size + 1));
    if (!g_env)
        return (0);
    i = 0;
    while (i < size)
    {
        g_env[i] = ft_strdup(envp[i]);
        if (!g_env[i])
        {
            ft_free_env(); // Ensure previously allocated memory is freed
            return (0);
        }
        i++;
    }
    g_env[i] = NULL;
    return (1);
}

static int ft_add_env(char *new_var)
{
    int i;
    char **new_env;

    if (!new_var) // Check if new_var is NULL
        return (0);

    i = 0;
    while (environ && environ[i])
        i++;
    new_env = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_env)
        return (0);
    i = 0;
    while (environ && environ[i])
    {
        new_env[i] = environ[i];
        i++;
    }
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    free(environ); // Free the old environment
    environ = new_env;
    return (1);
}

int ft_set_env(const char *name, const char *value)
{
    char *new_var;
    int i;

    if (!name || !value) // Check if name or value is NULL
        return (0);

    new_var = ft_strjoin3(name, "=", value);
    if (!new_var)
        return (0);
    i = 0;
    while (environ && environ[i])
    {
        if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0
            && environ[i][ft_strlen(name)] == '=')
        {
            free(environ[i]); // Free the old variable
            environ[i] = new_var;
            return (1);
        }
        i++;
    }
    return (ft_add_env(new_var));
}

int ft_unset_env(const char *name)
{
    int i;
    int j;

    if (!name) // Check if name is NULL
        return (0);

    i = 0;
    while (environ && environ[i])
    {
        if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0
            && environ[i][ft_strlen(name)] == '=')
        {
            free(environ[i]); // Free the variable to be unset
            j = i;
            while (environ[j])
            {
                environ[j] = environ[j + 1];
                j++;
            }
            return (1);
        }
        i++;
    }
    return (0);
}

void ft_free_env(void)
{
    int i;

    if (!g_env) // Check if g_env is NULL
        return;

    i = 0;
    while (g_env[i])
    {
        free(g_env[i]);
        i++;
    }
    free(g_env);
    g_env = NULL;
}
