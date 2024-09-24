/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/17 13:42:15 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/24 13:42:45 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "minishell.h" 

//legal func to print all different error msg
void	print_error(int fd, char *msg1, char *msg2, char cmd);

// Function to print command not found error
void	print_command_not_found(char *command);

// Function to print error for invalid exit arguments
void	print_exit_numeric_error(char *arg);

// Function to handle unset PATH error
void	print_unset_path_error(void);

// Function to handle file/directory not found errors
void	print_file_not_found_error(char *path);

// General function for handling permission errors
void	print_permission_error(char *path);

// Function for handling syntax errors (like unexpected tokens)
void	print_syntax_error(char *token);

// Function to print generic errors
void	print_generic_error(char *msg);

// Function for handling memory allocation errors
void	handle_memory_error(void);

#endif /* ERROR_H */
