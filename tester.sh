#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counter for passed and failed tests
PASSED=0
FAILED=0

# Minishell executable
MINISHELL="./minishell"

# Function to run a test
run_test() {
    TEST_NAME=$1
    COMMAND=$2
    EXPECTED_OUTPUT=$3
    EXPECTED_EXIT_CODE=$4

    echo "${YELLOW}Running test: $TEST_NAME${NC}"
    ACTUAL_OUTPUT=$(echo "$COMMAND" | $MINISHELL 2>&1 | sed '/minishell>/d; /exit/d')
    ACTUAL_EXIT_CODE=$?

    if [ "$ACTUAL_OUTPUT" = "$EXPECTED_OUTPUT" ] && [ $ACTUAL_EXIT_CODE -eq $EXPECTED_EXIT_CODE ]; then
        echo "${GREEN}Test passed${NC}"
        PASSED=$(($PASSED + 1))
    else
        echo "${RED}Test failed${NC}"
        echo "Expected output:\n$EXPECTED_OUTPUT"
        echo "Actual output:\n$ACTUAL_OUTPUT"
        echo "Expected exit code: $EXPECTED_EXIT_CODE"
        echo "Actual exit code: $ACTUAL_EXIT_CODE"
        FAILED=$(($FAILED + 1))
    fi
    echo
}

# ... (keep the rest of the script unchanged)

# Basic shell functionality
run_test "Simple echo" "echo Hello, World!" "Hello, World!" 0
run_test "Echo with multiple arguments" "echo Hello World from minishell" "Hello World from minishell" 0
run_test "Echo with quotes" "echo \"Hello, 'World'\"" "Hello, 'World'" 0

# Command execution
run_test "Simple ls" "ls -1 | sort | head -n 3" "$(ls -1 | sort | head -n 3)" 0
run_test "Command not found" "nonexistentcommand" "minishell: command not found: nonexistentcommand" 127
run_test "Execute command with full path" "/bin/echo Hello" "Hello" 0

# Built-in commands
run_test "Echo builtin" "echo test" "test" 0
run_test "Echo with -n option" "echo -n test" "test" 0
run_test "Pwd builtin" "pwd" "$(pwd)" 0
run_test "Cd to home" "cd; pwd" "$HOME" 0
run_test "Cd to specific directory" "cd /tmp; pwd" "/tmp" 0
run_test "Cd with relative path" "mkdir -p testdir; cd testdir; pwd; cd ..; rm -r testdir" "$(pwd)/testdir" 0
run_test "Export builtin" "export TEST=123; echo \$TEST" "123" 0
run_test "Unset builtin" "export TEST=123; unset TEST; echo \$TEST" "" 0
run_test "Env builtin" "env | grep PATH" "$(env | grep PATH)" 0

# Redirections
echo "test input" > test_input.txt
run_test "Input redirection" "cat < test_input.txt" "test input" 0
run_test "Output redirection" "echo test output > test_output.txt; cat test_output.txt" "test output" 0
run_test "Append output redirection" "echo first > test_append.txt; echo second >> test_append.txt; cat test_append.txt" "first\nsecond" 0
run_test "Here document" "cat << EOF
hello
world
EOF" "hello\nworld" 0

# Pipes
run_test "Simple pipe" "echo hello | cat" "hello" 0
run_test "Multiple pipes" "echo hello | sed 's/hello/hi/' | tr '[a-z]' '[A-Z]'" "HI" 0
run_test "Pipe with builtin" "echo hello | wc -c" "6" 0

# Environment variables
run_test "Environment variable expansion" "TEST=hello; echo \$TEST" "hello" 0
run_test "Environment variable in double quotes" "TEST=hello; echo \"\$TEST world\"" "hello world" 0
run_test "Multiple environment variables" "A=1; B=2; echo \$A \$B" "1 2" 0

# Exit status
run_test "Exit status of successful command" "true; echo \$?" "0" 0
run_test "Exit status of failed command" "false; echo \$?" "1" 0
run_test "Exit status in pipe" "false | true; echo \$?" "0" 0

# Quoting and metacharacter handling
run_test "Single quotes" "echo 'Hello      World'" "Hello      World" 0
run_test "Double quotes with space" "echo \"Hello      World\"" "Hello      World" 0
run_test "Double quotes with variable" "TEST=hello; echo \"\$TEST world\"" "hello world" 0
run_test "Ignore semicolon in quotes" "echo 'hello ; echo world'" "hello ; echo world" 0
run_test "Backslash escaping" "echo hello\\ world" "hello world" 0

# Complex commands
run_test "Complex command 1" "echo hello | tr '[a-z]' '[A-Z]' > test.txt; cat test.txt | sed 's/HELLO/MINISHELL/'" "MINISHELL" 0
run_test "Complex command 2" "export A=1; export B=2; echo \$((\$A + \$B))" "3" 0
run_test "Complex command 3" "cd /tmp; pwd | sed 's/tmp/temporary/'" "/temporary" 0

# Error handling
run_test "Directory not found" "cd /nonexistent" "minishell: cd: /nonexistent: No such file or directory" 1
run_test "Permission denied" "touch test_noperm.txt; chmod 000 test_noperm.txt; cat test_noperm.txt; rm -f test_noperm.txt" "minishell: cat: test_noperm.txt: Permission denied" 1

# Cleanup
rm -f test_input.txt test_output.txt test_append.txt test.txt

# Signal handling tests (manual)
echo -e "${YELLOW}Manual test required: Press Ctrl+C at the prompt. It should display a new prompt.${NC}"
echo -e "${YELLOW}Manual test required: Press Ctrl+D at the prompt. It should exit the shell.${NC}"
echo -e "${YELLOW}Manual test required: Press Ctrl+\\ at the prompt. It should do nothing.${NC}"

# Print summary
echo -e "${GREEN}Tests passed: $PASSED${NC}"
echo -e "${RED}Tests failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi