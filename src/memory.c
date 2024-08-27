#include "minishell.h"
#include <stdlib.h>

void free_command(t_command *cmd)
{
    int i;

    if (!cmd)
        return;

    if (cmd->path)
        free(cmd->path);
    
    if (cmd->argv)
    {
        for (i = 0; cmd->argv[i]; i++)
            free(cmd->argv[i]);
        free(cmd->argv);
    }

    if (cmd->redir)
    {
        for (i = 0; i < cmd->redirect_count; i++)
            free(cmd->redir[i].file);
        free(cmd->redir);
    }

    free(cmd);
}