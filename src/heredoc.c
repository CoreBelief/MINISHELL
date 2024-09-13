#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

// placeholder example heredoc bullshit EOF

// Function to read input from the user until the delimiter is reached
void heredoc_input(char *delimiter, char *tmp_filename) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int tmp_fd;

    // Open a temporary file to store the heredoc content
    tmp_fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (tmp_fd == -1) {
        perror("minishell: open");
        exit(EXIT_FAILURE);
    }

    // Prompt the user for input
    while (1) {
        printf("> ");
        read = getline(&line, &len, stdin);
        if (read == -1) {
            perror("minishell: getline");
            free(line);
            exit(EXIT_FAILURE);
        }

        // Remove the trailing newline
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        // Break if the user types the delimiter
        if (strcmp(line, delimiter) == 0) {
            break;
        }

        // Write the input to the temporary file
        write(tmp_fd, line, strlen(line));
        write(tmp_fd, "\n", 1);  // Add newline
    }

    free(line);
    close(tmp_fd);
}



// Of course! I’d be happy to help you implement a **heredoc** in your minishell. A **heredoc** allows the user to input multiple lines of text into a temporary file or stream, which can then be used as input for a command. The delimiter marks the end of the heredoc input.

// ### Concept of Heredoc

// The general steps to implement a heredoc:

// 1. **Prompt the user for input** until the delimiter is typed.
// 2. **Store the input in a temporary file** (or buffer) to be used as input for the child process.
// 3. **Replace the input redirection (`<<`) in the command** with the heredoc’s content.

// ### Steps to Implement Heredoc in C

// Here’s a basic flow of how to implement heredoc in your minishell.

// ### 1. **Detect Heredoc in the Command**
//    When parsing the command, check if it contains a heredoc (`<<`). If it does, store the delimiter provided by the user.

// ### 2. **Handle Input for the Heredoc**
//    - Create a temporary file (or a pipe, if you'd rather use a pipe for in-memory communication).
//    - Prompt the user repeatedly for input until they enter the delimiter.
//    - Write each line of input into the temporary file or the pipe.

// ### 3. **Redirect the Input**
//    - Before executing the command, ensure that the standard input is redirected to the file containing the heredoc's input (or the read end of the pipe).

// ---

// ### Example of Heredoc Implementation:

// Here’s a high-level implementation of a heredoc using a temporary file.

// #### 1. **Handling Heredoc Input:**

// ```c
// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <string.h>
// #include <stdlib.h>

// #define BUFFER_SIZE 1024

// // Function to read input from the user until the delimiter is reached
// void heredoc_input(char *delimiter, char *tmp_filename) {
//     char *line = NULL;
//     size_t len = 0;
//     ssize_t read;
//     int tmp_fd;

//     // Open a temporary file to store the heredoc content
//     tmp_fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     if (tmp_fd == -1) {
//         perror("minishell: open");
//         exit(EXIT_FAILURE);
//     }

//     // Prompt the user for input
//     while (1) {
//         printf("> ");
//         read = getline(&line, &len, stdin);
//         if (read == -1) {
//             perror("minishell: getline");
//             free(line);
//             exit(EXIT_FAILURE);
//         }

//         // Remove the trailing newline
//         if (line[read - 1] == '\n') {
//             line[read - 1] = '\0';
//         }

//         // Break if the user types the delimiter
//         if (strcmp(line, delimiter) == 0) {
//             break;
//         }

//         // Write the input to the temporary file
//         write(tmp_fd, line, strlen(line));
//         write(tmp_fd, "\n", 1);  // Add newline
//     }

//     free(line);
//     close(tmp_fd);
// }
// ```

// #### 2. **Integrating Heredoc into the Execution Process:**

// - When parsing the command and detecting `<<`, call `heredoc_input()` to capture the input and store it in a temporary file.
  
// - Then, before executing the command, **redirect the input** to the heredoc file (in the child process).

// Here’s how you would modify the command execution to handle the heredoc.

// ```c
// void	handle_child_process(t_command *cmd, int pipe_fds[2], int prev_pipe_read, t_shell *shell)
// {
// 	// Check for heredoc in the command
// 	if (cmd->heredoc) {
// 		// Set up the heredoc and store its input in a temporary file
// 		char *heredoc_filename = "/tmp/minishell_heredoc";
// 		heredoc_input(cmd->heredoc_delimiter, heredoc_filename);

// 		// Redirect the input to the heredoc file
// 		int heredoc_fd = open(heredoc_filename, O_RDONLY);
// 		if (heredoc_fd == -1) {
// 			perror("minishell: open heredoc file");
// 			exit(EXIT_FAILURE);
// 		}
// 		dup2(heredoc_fd, STDIN_FILENO);
// 		close(heredoc_fd);
// 	}

// 	// Usual redirection and command execution logic
// 	if (cmd->redirect_count)
// 		setup_redirections(cmd);
// 	if (prev_pipe_read != -1 && cmd->input == -1)
// 	{
// 		dup2(prev_pipe_read, STDIN_FILENO);
// 		close(prev_pipe_read);
// 	}
// 	if (cmd->pipe_out == 1 && cmd->output == -1)
// 	{
// 		dup2(pipe_fds[1], STDOUT_FILENO);
// 		close(pipe_fds[1]);
// 	}
// 	close(pipe_fds[0]);
// 	if (is_builtin(cmd->argv[0]))
// 	{
// 		execute_builtin(cmd, shell);
// 		exit(shell->last_exit_status);
// 	}
// 	else
// 	{
// 		execute_external(cmd, shell);
// 	}
// 	exit(EXIT_FAILURE);
// }
// ```

// ### 3. **Handling Cleanup**
// Once the heredoc file is no longer needed, you should clean it up to avoid leaving temporary files behind:

// ```c
// unlink("/tmp/minishell_heredoc");  // Clean up heredoc file
// ```

// You can call this after the command finishes executing (in the parent process, after `wait_for_children()`).

// ### 4. **Extend Command Parsing for `<<`**

// You’ll also need to modify your command parsing logic to handle the heredoc operator (`<<`) and capture the delimiter. For example, during parsing:

// ```c
// if (strcmp(token, "<<") == 0) {
//     cmd->heredoc = 1;
//     cmd->heredoc_delimiter = strdup(next_token);  // Store the delimiter
// }
// ```

// ### Summary of Key Points:
// - **Detect the `<<` operator** during command parsing.
// - **Prompt the user** for input until the delimiter is typed, and **store the input in a temporary file** or buffer.
// - **Redirect the input** from the file when executing the command.
// - **Clean up** the temporary file after the command completes.

// This approach can be further enhanced, but it provides a basic working implementation of heredoc functionality in your shell. Let me know if you need help with the parsing, or if you'd prefer a pipe-based solution!