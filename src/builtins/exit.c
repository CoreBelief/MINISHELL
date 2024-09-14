#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int is_space(char c) 
{
    return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static int is_digit(char c) 
{
    return (c >= '0' && c <= '9');
}

int is_valid_number(const char *str, long *number) 
{
    int i = 0;
    int sign = 1;
    unsigned long result = 0;

    while (is_space(str[i])) 
        i++;
    if (str[i] == '+' || str[i] == '-') 
	{
        if (str[i] == '-') 
            sign = -1;
        i++;
    }
    if (!is_digit(str[i])) 
        return 0; // Invalid number
    while (str[i] != '\0') {
        if (!is_digit(str[i]))
            return 0; // Non-digit character found
        int digit = str[i] - '0';
        if (result > (ULONG_MAX - digit) / 10)
            return 0; // Overflow detected
        result = result * 10 + digit;
        i++;
    }
     if (sign == -1 && result > (unsigned long)LONG_MAX + 1) 
        return 0; // Underflow detected
    if (sign == 1 && result > (unsigned long)LONG_MAX)
        return 0; // Overflow detected
    *number = sign * (long)result;
    return 1; // Valid number
}

void builtin_exit(char **args) {
    long exit_status;

    printf("exit\n");
    if (!args[1]) {
        exit(0);
    }
    if (args[2]) {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        return;
    }
    if (!is_valid_number(args[1], &exit_status)) {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
        exit(255);
    }
    exit((int)(exit_status & 255));
}
