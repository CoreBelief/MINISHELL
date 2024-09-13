#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// Function to print command not found error
void print_command_not_found(char *command);

// Function to print error for invalid exit arguments
void print_exit_numeric_error(char *arg);

// Function to handle unset PATH error
void print_unset_path_error();

// Function to handle file/directory not found errors
void print_file_not_found_error(char *path);

// General function for handling permission errors
void print_permission_error(char *path);

// Function for handling syntax errors (like unexpected tokens)
void print_syntax_error(char *token);

// Function to print generic errors
void print_generic_error(char *msg);

// Function for handling memory allocation errors
void handle_memory_error();

#endif /* ERROR_H */
