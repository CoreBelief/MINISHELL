#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counter for passed and failed tests
PASSED=0
FAILED=0

# Function to run a test
run_test() {
    echo -e "${YELLOW}Testing: $1${NC}"
    if eval "$2"; then
        echo -e "${GREEN}Passed${NC}"
        ((PASSED++))
    else
        echo -e "${RED}Failed${NC}"
        ((FAILED++))
    fi
    echo
}

# Function to compare output with expected result
compare_output() {
    expected="$1"
    command="$2"
    output=$(echo "$command" | ./minishell 2>&1 | sed -e 's/^rdl@rtx:.*\$ //' -e '/^rdl@rtx:.*\$ exit$/d')
    if [ "$output" = "$expected" ]; then
        return 0
    else
        echo "Expected: $expected"
        echo "Got: $output"
        return 1
    fi
}

# Start testing
echo "Starting Minishell tests..."

# Test simple commands
run_test "Simple command (ls)" "compare_output '$(ls)' 'ls'"
run_test "Simple command with arguments (ls -l)" "compare_output '$(ls -l)' 'ls -l'"

# Test empty commands and whitespace
run_test "Empty command" "compare_output '' ''"
run_test "Whitespace command" "compare_output '' '   '"

# Test redirections
echo "test content" > test_file.txt
run_test "Input redirection" "compare_output 'test content' 'cat < test_file.txt'"
run_test "Output redirection" "echo 'output test' > out.txt && cat out.txt | compare_output 'output test' 'cat'"
run_test "Append redirection" "echo 'append test' >> out.txt && tail -n 1 out.txt | compare_output 'append test' 'cat'"

# Test environment variables
run_test "env command" "compare_output '$(env | sort)' 'env | sort'"
run_test "export command" "export TEST_VAR='test value' && env | grep TEST_VAR | compare_output 'TEST_VAR=test value' 'cat'"

# Test echo
run_test "echo command" "compare_output 'Hello, World!' 'echo Hello, World!'"
run_test "echo with -n option" "compare_output -n 'Hello, World!' 'echo -n Hello, World!'"

# Test arguments
run_test "Command with arguments" "compare_output '$(ls -la)' 'ls -la'"

# Test PATH handling
original_path=$PATH
run_test "Unset PATH" "export PATH='' && compare_output 'minishell: execve failed: No such file or directory' 'ls'"
export PATH=$original_path

# Test exit command
run_test "exit command" "echo 'exit' | ./minishell > /dev/null && echo 'Minishell exited' | compare_output 'Minishell exited' 'cat'"

# Test pipes
run_test "Simple pipe" "compare_output 'hello' 'echo hello | cat'"
run_test "Multiple pipes" "compare_output 'HELLO' 'echo hello | cat | tr a-z A-Z'"

# Test quotes
run_test "Single quotes" "compare_output '$USER' 'echo '$USER''"
run_test "Double quotes with variable" "compare_output $USER 'echo \"$USER\"'"

# Test pwd
run_test "pwd command" "compare_output '$(pwd)' 'pwd'"

# Test return value
run_test "Return value of successful command" "ls > /dev/null && compare_output '0' 'echo $?'"
run_test "Return value of failed command" "ls non_existent_file > /dev/null 2>&1 && compare_output '2' 'echo $?'"

# Test relative path
run_test "Relative path" "compare_output 'test' '../../../bin/echo test'"

# Test unset
run_test "unset command" "export TEST_VAR='test' && unset TEST_VAR && env | grep TEST_VAR | compare_output '' 'cat'"

# Test cd
run_test "cd command" "cd .. && compare_output '$(dirname $(pwd))' 'pwd'"

# Clean up
rm -f test_file.txt out.txt

# Print test results
echo -e "${GREEN}Passed tests: $PASSED${NC}"
echo -e "${RED}Failed tests: $FAILED${NC}"

# Manual test instructions
echo -e "${YELLOW}Manual tests required:${NC}"
echo "1. Type a command, use ctrl-C, and press Enter. The buffer should be cleared."
echo "2. Use Up and Down arrows to navigate through history."
echo "3. Try executing a non-existent command (e.g., 'asdfghjkl')."
echo "4. Execute 'cat | cat | ls' and observe the behavior."
echo "5. Try ctrl-C, ctrl-D, and ctrl-\ in an empty prompt and with text."
echo "6. Try these signals with a blocking command like 'cat' without arguments."

# Exit with success if all tests passed, failure otherwise
[ $FAILED -eq 0 ]