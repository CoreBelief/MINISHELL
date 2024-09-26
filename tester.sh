#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Function to run test and check result
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    local expected_exit_code="$4"

    echo "Testing $test_name..."
    output=$(echo "$command" | ./minishell 2>&1)
    exit_code=$?

    if [[ "$output" =~ $expected_output ]] && [[ $exit_code -eq $expected_exit_code ]]; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "Expected output to match: $expected_output"
        echo "Actual output: $output"
        echo "Expected exit code: $expected_exit_code"
        echo "Actual exit code: $exit_code"
    fi
    echo
}

# BUILTIN/CD tests
run_test "CD to root" "cd /../../../../../..\npwd" "^/$" 0
run_test "CD to home" "cd ~\npwd" "^/home/[a-zA-Z0-9]+$" 0
run_test "CD without args" "cd\npwd" "^/home/[a-zA-Z0-9]+$" 0
run_test "CD with unset HOME" "unset HOME\ncd" "HOME not set" 1
run_test "CD to non-existent dir" "cd blabla" "No such file or directory" 1

# BUILTIN/ECHO tests
run_test "Echo without args" "echo" "^$" 0
run_test "Echo with -n" "echo -n" "^$" 0
run_test "Echo non-existing var" "echo \$NonExistingVar" "^$" 0
run_test "Echo PATH" "echo \$PATH" "/usr/local/(s)?bin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" 0
run_test "Echo home" "echo ~" "^/home/[a-zA-Z0-9]+$" 0

# BUILTIN/ENV tests
run_test "Env without args" "env" "PATH=" 0
run_test "Env with non-existent command" "env blabla" "No such file or directory" 127

# BUILTIN/EXPORT tests
run_test "Export without args" "export" "declare -x" 0
run_test "Export multiple vars" "export var1=a var2=b var3=c\nenv | grep 'var[1-3]'" "var1=a.*var2=b.*var3=c" 0
run_test "Export invalid identifier" "export 1var" "not a valid identifier" 1

# EXECUTION tests
run_test "Execute non-existent command" "random_cmd" "command not found" 127
run_test "Execute non-executable file" "touch non_executable_file && ./non_executable_file" "Permission denied" 126

# PARSING tests
run_test "Parse empty string" '""' "command not found" 127
run_test "Parse quoted string" 'echo "test | test ; alsotest >!<"' "test | test ; alsotest >!<" 0

# PIPE tests
run_test "Simple pipe" "ls | wc -l" "[0-9]+" 0

# REDIRECTION tests
run_test "Output redirection" "echo hello > test.txt && cat test.txt" "hello" 0
run_test "Input redirection" "echo world > test.txt && < test.txt cat" "world" 0

# Clean up
rm -f test.txt non_executable_file

echo "All tests completed."