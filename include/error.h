/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/17 13:42:15 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/26 19:25:14 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "minishell.h" 

//legal func to print all different error msg
void	print_error(char *cmd, char *msg);

// Function to print command not found error
void	print_command_not_found(char *command);

// Function to print error for invalid exit arguments
void	print_exit_numeric_error(char *arg);

#endif /* ERROR_H */
