/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 19:20:16 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 18:40:04 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input_redirection(int prev_prd, t_cmd *cmd);
static void	handle_output_redirection(int pfds[2], t_cmd *cmd);
static void	close_pipe_ends(int pfds[2]);
void		child_proc(t_cmd *cmd, int pipe_fds[2],
				int prev_pipe_read, t_shell *shell);
void		parent_proc(t_cmd *cmd, int pipe_fds[2],
				int *prev_pipe_read);

static void	handle_input_redirection(int prev_prd, t_cmd *cmd)
{
	if (prev_prd != -1 && cmd->input == -1)
	{
		if (dup2(prev_prd, STDIN_FILENO) == -1)
		{
			perror("dup2 failed for STDIN");
			exit(EXIT_FAILURE);
		}
		close(prev_prd);
	}
}

static void	handle_output_redirection(int pfds[2], t_cmd *cmd)
{
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		if (dup2(pfds[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for STDOUT");
			exit(EXIT_FAILURE);
		}
		close(pfds[1]);
	}
}

static void	close_pipe_ends(int pfds[2])
{
	if (pfds[0] != -1)
		close(pfds[0]);
	if (pfds[1] != -1)
		close(pfds[1]);
}

// void	child_proc(t_cmd *cmd, int pfds[2], int prev_prd, t_shell *shell)
// {
// 	setup_signals_child();
// 	if (cmd->redirect_count)
// 		setup_redirections(cmd);
// 	handle_input_redirection(prev_prd, cmd);
// 	handle_output_redirection(pfds, cmd);
// 	close_pipe_ends(pfds);
// 	// fprintf(stderr, "exe does :: %S", cmd->argv[0]);
// 	if (is_builtin(cmd->argv[0]))
// 	{
// 		execute_builtin(cmd, shell);
// 		exit(shell->last_exit_status);
// 	}
// 	else
// 		execute_external(cmd, shell);
// 	exit(EXIT_FAILURE);
// }
void child_proc(t_cmd *cmd, int pfds[2], int prev_prd, t_shell *shell)
{
    fprintf(stderr, "DEBUG: Entering child process\n");
    setup_signals_child();
    
    if (cmd->redirect_count)
    {
        fprintf(stderr, "DEBUG: Setting up redirections\n");
        setup_redirections(cmd);
    }
    
    handle_input_redirection(prev_prd, cmd);
    handle_output_redirection(pfds, cmd);
    close_pipe_ends(pfds);
    
    fprintf(stderr, "DEBUG: Executing command: %s\n", cmd->argv[0]);
    for (int i = 0; cmd->argv[i] != NULL; i++)
    {
        fprintf(stderr, "DEBUG: argv[%d] = %s\n", i, cmd->argv[i]);
    }
    
    if (is_builtin(cmd->argv[0]))
    {
        fprintf(stderr, "DEBUG: Executing builtin command\n");
        execute_builtin(cmd, shell);
        fprintf(stderr, "DEBUG: Builtin execution complete, exit status: %d\n", shell->last_exit_status);
        exit(shell->last_exit_status);
    }
    else
    {
        fprintf(stderr, "DEBUG: Executing external command\n");
        execute_external(cmd, shell);
    }
    
    fprintf(stderr, "DEBUG: Command execution failed\n");
    exit(EXIT_FAILURE);
}
void	parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd)
{
	if (*prev_prd != -1)
	{
		close(*prev_prd);
	}
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		close(pfds[1]);
		*prev_prd = pfds[0];
	}
	else
	{
		*prev_prd = -1;
		close_pipe_ends(pfds);
	}
}
