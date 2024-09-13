#!/bin/sh

MINISHELL="./minishell"
ORANGE='\033[0;33m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

test_count=0
pass_count=0
failed_tests=""

remove_prompts() {
    sed '/^.*@.*:.*[$#] /d' "$1" > "$1.tmp" && mv "$1.tmp" "$1"
}

run_test() {
    test_name="$1"
    command="$2"
    
    test_count=$((test_count + 1))
    
    # Run command in minishell
    printf "%s\n" "$command" | $MINISHELL > minishell_output 2>&1
    
    # Run command in sh for comparison
    printf "%s\n" "$command" | sh > sh_output 2>&1
    
    # Remove prompt lines from both outputs
    remove_prompts minishell_output
    remove_prompts sh_output
    
    # Compare outputs
    if diff -q minishell_output sh_output >/dev/null 2>&1; then
        pass_count=$((pass_count + 1))
    else
        failed_tests="$failed_tests$test_name\n"
        printf "${RED}FAIL: %s${NC}\n" "$test_name"
        printf "Command: %s\n" "$command"
        printf "Expected output (sh):\n"
        cat sh_output
        printf "Actual output (minishell):\n"
        cat minishell_output
        printf "\n"
    fi
}

# Test cases
run_test "Simple command" "ls"
run_test "Command with arguments" "ls -l"
run_test "Non-existent command" "nonexistentcommand"
run_test "Echo environment variable" "echo \$HOME"
run_test "Set environment variable" "export TEST_VAR=hello"
run_test "Echo set environment variable" "echo \$TEST_VAR"
run_test "Unset environment variable" "unset TEST_VAR"
run_test "Echo unset environment variable" "echo \$TEST_VAR"
run_test "Output redirection" "echo Hello > test_file.txt"
run_test "Cat redirected file" "cat test_file.txt"
run_test "Input redirection" "cat < test_file.txt"
run_test "Append redirection" "echo World >> test_file.txt"
run_test "Cat appended file" "cat test_file.txt"
run_test "Here document" "cat << EOF
Hello
World
EOF"
run_test "Simple pipe" "echo 'hello world' | tr 'a-z' 'A-Z'"
run_test "Multiple pipes" "ls | grep test | wc -l"
run_test "Single quotes" "echo 'Hello    World'"
run_test "Double quotes" "echo \"Hello    World\""
run_test "Mixed quotes" "echo \"'Hello'    'World'\""
run_test "Environment variable in double quotes" "echo \"\$USER\""
run_test "Environment variable in single quotes" "echo '\$USER'"
run_test "echo command" "echo Hello World"
run_test "echo with -n option" "echo -n Hello World"
run_test "pwd command" "pwd"
run_test "cd command" "cd /tmp"
run_test "pwd after cd" "pwd"
run_test "env command" "env | head -n 5"
run_test "export command" "export NEW_VAR=value"
run_test "echo exported variable" "echo \$NEW_VAR"
run_test "unset command" "unset NEW_VAR"
run_test "echo unset variable" "echo \$NEW_VAR"
run_test "Exit status of successful command" "ls /tmp"
run_test "Echo exit status" "echo \$?"
run_test "Exit status of failed command" "ls /nonexistent"
run_test "Echo failed exit status" "echo \$?"
run_test "Pipe and redirection" "cat /etc/passwd | grep root > root_info.txt"
run_test "Cat result of pipe and redirection" "cat root_info.txt"
run_test "Multiple redirections" "echo Hello > file1"
run_test "Second redirection" "echo World > file2"
run_test "Combine redirected files" "cat file1 file2 > combined"
run_test "Cat combined file" "cat combined"
# run_test "Large command" "echo $(seq 1 1000)"
# run_test "Many pipes" "echo hello | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat"

# Clean up
rm -f test_file.txt file1 file2 combined root_info.txt

# Print summary
printf "\n${ORANGE}Test Summary:${NC}\n"
printf "Total tests: %d\n" $test_count
printf "Passed tests: %d\n" $pass_count
printf "Failed tests: %d\n" $((test_count - pass_count))

if [ $pass_count -eq $test_count ]; then
    printf "${GREEN}All tests passed!${NC}\n"
else
    printf "${RED}Failed tests:${NC}\n"
    printf "%b" "$failed_tests"
fi