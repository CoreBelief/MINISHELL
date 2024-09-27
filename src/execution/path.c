#include "minishell.h"

char	*search_paths(char **paths, char *command);
char	*find_executable(char *command, t_shell *shell);
char	*find_command_in_path(char *command, t_shell *shell);


char	*search_paths(char **paths, char *command)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
char *find_executable(char *command, t_shell *shell)
{
    char *path_env;
    char **paths;
    char *full_path;
    int i;

    // Check if the command is an absolute or relative path (starts with '/' or '.')
    if (command[0] == '/' || command[0] == '.')
    {
        // Check if the file exists
        if (access(command, F_OK) == 0)
        {
            // Check if the file is executable
            if (access(command, X_OK) == 0)
                return (ft_strdup(command));  // Return the command if it's executable
            else
            {
                shell->last_exit_status = 126;  // Permission denied
                return NULL;  // Let the calling function print the error
            }
        }
        shell->last_exit_status = 127;  // File not found
        return NULL;  // Let the calling function print the error
    }

    // Get the PATH environment variable
    path_env = ft_get_env("PATH", shell);
    if (!path_env)
        return NULL;

    // Split the PATH variable by colon ':'
    paths = ft_split(path_env, ':');
    if (!paths)
        return NULL;

    // Loop through the directories in PATH
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(full_path, command);

        // Check if the command exists and is executable
        if (access(full_path, X_OK) == 0)
        {
            ft_free_str_array(paths);
            return full_path;  // Return the full path if executable is found
        }
        free(full_path);
        i++;
    }

    // Free the array of paths and handle command not found
    ft_free_str_array(paths);

    shell->last_exit_status = 127;  // Command not found
    return NULL;  // Let the calling function print the error
}

// char *find_executable(char *command, t_shell *shell)
// {
//     char *path_env;
//     char **paths;
//     char *full_path;
//     int i;

//     // Check if the command is an absolute or relative path (starts with '/' or '.')
//     if (command[0] == '/' || command[0] == '.')
//     {
//         // Check if the file exists
//         if (access(command, F_OK) == 0)
//         {
//             // Check if the file is executable
//             if (access(command, X_OK) == 0)
//                 return (ft_strdup(command));  // Return the command if it's executable
//             else
//             {
//                 // Permission denied error
//                 shell->last_exit_status = 126;  // Permission denied
//                 ft_putstr_fd(command, 2);
//                 ft_putstr_fd(": Permission denied\n", 2);
//                 return NULL;
//             }
//         }
//         // If the file doesn't exist, return the appropriate error
//         shell->last_exit_status = 127;  // File not found
//         ft_putstr_fd(command, 2);
//         ft_putstr_fd(": No such file or directory\n", 2);
//         return NULL;
//     }

//     // Get the PATH environment variable
//     path_env = ft_get_env("PATH", shell);
//     if (!path_env)
//         return NULL;

//     // Split the PATH variable by colon ':'
//     paths = ft_split(path_env, ':');
//     if (!paths)
//         return NULL;

//     // Loop through the directories in PATH
//     i = 0;
//     while (paths[i])
//     {
//         full_path = ft_strjoin(paths[i], "/");
//         full_path = ft_strjoin(full_path, command);

//         // Check if the command exists and is executable
//         if (access(full_path, X_OK) == 0)
//         {
//             ft_free_str_array(paths);
//             return full_path;  // Return the full path if executable is found
//         }
//         free(full_path);
//         i++;
//     }

//     // Free the array of paths and handle command not found
//     ft_free_str_array(paths);

//     // If the command isn't found in any of the PATH directories
//     shell->last_exit_status = 127;
//     ft_putstr_fd(command, 2);
//     ft_putstr_fd(": command not found\n", 2);
//     return NULL;
// }


char	*find_command_in_path(char *command, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	struct stat	statbuf;

	if (ft_strchr(command, '/'))
	{
		if (stat(command, &statbuf) == -1)
		{
			if (errno == ENOENT)
				shell->last_exit_status = 127;
			else
				shell->last_exit_status = 126;
			return (NULL);
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			shell->last_exit_status = 126;
			errno = EISDIR; // is dir
			return (NULL);
		}
		if (access(command, X_OK) == -1)
		{
			shell->last_exit_status = 126;
			return (NULL);
		}
		return (ft_strdup(command));
	}
	path_env = ft_get_env("PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = search_paths(paths, command);
	ft_free_str_array(paths);
	if (full_path)
		return (full_path);
	shell->last_exit_status = 127;
	return (NULL);
}

// int	check_permissions(char *command, t_shell *shell)
// {
// 	struct stat	statbuf;

// 	if (ft_strchr(command, '/'))
// 	{
// 		if (stat(command, &statbuf) == -1)
// 		{
// 			shell->last_exit_status = 126;
// 			ft_putstr_fd(" Error opening file\n", 2);
// 			exit(shell->last_exit_status);
// 		}
// 		if (stat(command, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
// 		{
// 			shell->last_exit_status = 126;
// 			ft_putstr_fd(" Is a directory\n", 2);
// 			exit(shell->last_exit_status);
// 		}
// 		if (access(command, X_OK) == -1)
// 		{
// 			shell->last_exit_status = 126;
// 			ft_putstr_fd(" Permission denied\n", 2);
// 			exit(shell->last_exit_status);
// 		}
// 	}
// 	return (1);
// }

int	check_permissions(char *command, t_shell *shell)
{
	struct stat	statbuf;

	// Check if the command includes a '/'
	if (ft_strchr(command, '/'))
	{
		// Use stat to get file info
		if (stat(command, &statbuf) == -1)
		{
			if (errno == ENOENT)
			{
				shell->last_exit_status = 127;  // File not found
				ft_putstr_fd(command, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
			}
			else
			{
				shell->last_exit_status = 126;  // Other error
				ft_putstr_fd(command, 2);
				ft_putstr_fd(": Error opening file\n", 2);
			}
			exit(shell->last_exit_status);
		}

		// Check if the file is a directory
		if (S_ISDIR(statbuf.st_mode))
		{
			shell->last_exit_status = 126;  // Is a directory
			ft_putstr_fd(command, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(shell->last_exit_status);
		}

		// Check for execute permissions
		if (access(command, X_OK) == -1)
		{
			shell->last_exit_status = 126;  // Permission denied
			ft_putstr_fd(command, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(shell->last_exit_status);
		}
	}
	return (1);  // Return success if all checks pass
}

