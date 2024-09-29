#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

// Function prototypes
static int is_space(char c);
static int is_digit(char c);
static int is_valid_number(const char *str, long *number);
void builtin_exit(char **args);

// Helper function to check for spaces
static int is_space(char c) {
    return (c == ' ' || (c >= '\t' && c <= '\r'));
}

// Helper function to check for digits
static int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

// Function to validate if the string is a valid number
static int is_valid_number(const char *str, long *number) {
    int i = 0;
    int sign = 1;
    unsigned long result = 0;

    // Skip leading spaces
    while (is_space(str[i]))
        i++;
    // Handle sign
    if (str[i] == '+' || str[i] == '-') {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    // If no digits after the sign, return false
    if (!is_digit(str[i]))
        return 0;
    // Parse digits
    while (str[i] != '\0') {
        if (!is_digit(str[i]))
            return 0;
        int digit = str[i] - '0';
        if (result > (ULONG_MAX - digit) / 10)
            return 0;
        result = result * 10 + digit;
        i++;
    }
    // Check for overflow
    if (sign == -1 && result > (unsigned long)LONG_MAX + 1)
        return 0;
    if (sign == 1 && result > (unsigned long)LONG_MAX)
        return 0;
    *number = sign * (long)result;
    return 1;
}

// Improved exit command implementation
void builtin_exit(char **args) {
    long exit_status;
    int numeric_status = 0;

    // Print exit message
    printf("exit\n");

    // If no argument is provided, exit with status of last command
    if (!args[1]) {
        exit(0);
    }

    // Remove quotes from the argument if needed (assuming you have a remove_quotes function)
    remove_quotes(args[1]);

    // Check for multiple arguments (bash does not exit in this case)
    if (args[2]) {
        ft_putstr_fd("exit: too many arguments\n", 2);
        return;  // Do not exit, just return
    }

    // Check if the argument is a valid number
    numeric_status = is_valid_number(args[1], &exit_status);
    if (!numeric_status) {
        // Non-numeric argument error, exit with code 255
        fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
        exit(255);
    }

    // Exit with the numeric status modulo 256
    exit((int)(exit_status & 255));
}



// #include <limits.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <ctype.h>
// #include <string.h>
// #include "parser.h"
// static int is_space(char c);
// static int is_digit(char c);
// static int is_valid_number(const char *str, long *number);
// void builtin_exit(char **args);

// //change to correct error codes in builtin_exit
// static int is_space(char c) 
// {
//     return (c == ' ' || (c >= '\t' && c <= '\r'));
// }

// static int is_digit(char c) 
// {
//     return (c >= '0' && c <= '9');
// }

// static int is_valid_number(const char *str, long *number) 
// {
//     int i = 0;
//     int sign = 1;
//     unsigned long result = 0;

//     while (is_space(str[i])) 
//         i++;
//     if (str[i] == '+' || str[i] == '-') 
//     {
//         if (str[i] == '-') 
//             sign = -1;
//         i++;
//     }
//     if (!is_digit(str[i])) 
//         return 0;
//     while (str[i] != '\0') 
//     {
//         if (!is_digit(str[i])) 
//             return 0;
//         int digit = str[i] - '0';
//         if (result > (ULONG_MAX - digit) / 10) 
//             return 0;
//         result = result * 10 + digit;
//         i++;
//     }
//     if (sign == -1 && result > (unsigned long)LONG_MAX + 1) 
//         return 0;
//     if (sign == 1 && result > (unsigned long)LONG_MAX) 
//         return 0;
//     *number = sign * (long)result;
//     return 1;
// }

// void builtin_exit(char **args) 
// {
//     long exit_status;
//     int numeric_status = 0;

//     printf("exit\n");
//     if (!args[1]) 
//         exit(0);
//     remove_quotes(args[1]);
//     if (args[2]) 
//     {
//         ft_putstr_fd("exit: too many arguments\n", 2); //make more integrated?
//         exit(1);
//     }
//     numeric_status = is_valid_number(args[1], &exit_status);
//     if (!numeric_status) 
//     {
//         print_exit_numeric_error(args[1]);
//         exit(2);
//     }
//     exit((int)(exit_status & 255));
// }