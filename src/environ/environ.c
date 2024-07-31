#include "environ.h"
#include "libft.h"
#include "utils.h"
#include <stdlib.h>

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

char *ft_get_env(const char *name)
{
    int i;
    size_t len;

    if (!name) // Check if name is NULL
        return (NULL);
    
    i = 0;
    len = ft_strlen(name);
    while (g_env && g_env[i]) // Ensure g_env is not NULL
    {
        if (ft_strncmp(g_env[i], name, len) == 0 && g_env[i][len] == '=')
            return (g_env[i] + len + 1);
        i++;
    }
    return (NULL);
}

static int ft_add_env(char *new_var)
{
    int i;
    char **new_env;

    if (!new_var) // Check if new_var is NULL
        return (0);

    i = 0;
    while (g_env && g_env[i])
        i++;
    new_env = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_env)
        return (0);
    i = 0;
    while (g_env && g_env[i])
    {
        new_env[i] = g_env[i];
        i++;
    }
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    free(g_env); // Free the old environment
    g_env = new_env;
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
    while (g_env && g_env[i])
    {
        if (ft_strncmp(g_env[i], name, ft_strlen(name)) == 0
            && g_env[i][ft_strlen(name)] == '=')
        {
            free(g_env[i]); // Free the old variable
            g_env[i] = new_var;
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
    while (g_env && g_env[i])
    {
        if (ft_strncmp(g_env[i], name, ft_strlen(name)) == 0
            && g_env[i][ft_strlen(name)] == '=')
        {
            free(g_env[i]); // Free the variable to be unset
            j = i;
            while (g_env[j])
            {
                g_env[j] = g_env[j + 1];
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
