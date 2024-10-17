echo hello world
ls -la
echo "Hello, world!" > test.txt
cat /etc/passwd | grep root
echo "This is a \"quoted\" string with a \$ sign"
ls *.txt
echo $HOME
cat << EOF > output.txt
This is a multi-line
text block
EOF
wc -l < /etc/passwd
echo {1..5}
/bin/echo "Using full path"
grep error < input.log > output.txt 2> error.log