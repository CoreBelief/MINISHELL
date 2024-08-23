#!/bin/sh

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "Starting Minishell Tests..."

# Function to run a command in minishell and compare output
run_minishell_test() {
    expected_output="$1"
    command="$2"
    echo "${YELLOW}Testing: $command${NC}"
    output=$(echo "$command" | ./minishell 2>&1 | sed 's/^.*\$ //')
    if echo "$output" | grep -q "$expected_output"; then
        echo "${GREEN}Test passed${NC}"
    else
        echo "${RED}Test failed${NC}"
        echo "Expected: $expected_output"
        echo "Got: $output"
    fi
    echo
}

# Basic command execution
run_minishell_test "$(ls)" "ls"

# Echo command
run_minishell_test "Hello, World!" "echo Hello, World!"
run_minishell_test "No newline" "echo -n No newline"

# Environment variables
run_minishell_test "/home/" "echo \$HOME"

# Redirections
echo "Test" > test.txt
run_minishell_test "Test" "cat < test.txt"
run_minishell_test "Overwrite" "echo Overwrite > test.txt && cat test.txt"
run_minishell_test "Append" "echo Append >> test.txt && cat test.txt"

# Pipes
run_minishell_test "HELLO" "echo hello | tr a-z A-Z"

# Quotes
run_minishell_test "This \$HOME should not expand" "echo 'This \$HOME should not expand'"
run_minishell_test "/home/" "echo \"This \$HOME should expand\""

# Built-in commands
run_minishell_test "$(pwd)" "pwd"
run_minishell_test "/" "cd / && pwd"
run_minishell_test "TestVar" "export TEST_VAR=TestVar && echo \$TEST_VAR"
run_minishell_test "" "export TEST_VAR=Hello && unset TEST_VAR && echo \$TEST_VAR"
run_minishell_test "PATH=" "env | grep PATH"

# Exit status
run_minishell_test "1" "false && echo \$?"

# Command separation
run_minishell_test "Hello\nWorld" "echo Hello; echo World"

# Complex command
run_minishell_test "HELLO" "echo Hello > file.txt; cat < file.txt | tr a-z A-Z | tee output.txt; cat output.txt"

# Cleanup
rm -f test.txt file.txt output.txt

echo "${YELLOW}All tests completed.${NC}"