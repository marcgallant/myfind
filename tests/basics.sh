#!/bin/sh

touch find.txt
touch myfind.txt

test_suite () {
    ACTUAL=0
    EXPECTED=0
    cd ..
    find $@ > tests/find.txt
    EXPECTED=$(echo $?)
    EXPECTED=$((EXPECTED+0))
    ./myfind $@ > tests/myfind.txt
    ACTUAL=$(echo $?)
    ACTUAL=$((ACTUAL+0))
    if [ "$ACTUAL" -ne "$EXPECTED" ]; then
        echo "Failed test: $@"
    fi
    cd tests
    diff 'find.txt' 'myfind.txt'
}

test_suite_depth () {
    ACTUAL=0
    EXPECTED=0
    cd ..
    find $@ -depth> tests/find.txt
    EXPECTED=$(echo $?)
    EXPECTED=$((EXPECTED+0))
    ./myfind -d $@ > tests/myfind.txt
    ACTUAL=$(echo $?)
    ACTUAL=$((ACTUAL+0))
    if [ "$ACTUAL" -ne "$EXPECTED" ]; then
        echo "Failed test: $@"
    fi
    cd tests
    diff 'find.txt' 'myfind.txt'
}
echo "Basics"

echo "Test[1]"
test_suite
echo "Test[2]"
test_suite ~
echo "Test[3]"
test_suite .
echo "Test[4]"
test_suite ..
echo "Test[5]"
test_suite src tests ../
echo "Test[6]"
test_suite src/ tests/ ../
echo "Test[7]"
test_suite ewfewf

echo "Type"

echo "Test[1]"
test_suite /dev -type b
echo "Test[2]"
test_suite /dev -type
echo "Test[3]"
test_suite /dev -type c
echo "Test[4]"
test_suite /dev -type d
echo "Test[5]"
test_suite /dev -type f
echo "Test[6]"
test_suite /dev -type l
echo "Test[7]"
test_suite /dev -type p
echo "Test[8]"
test_suite /dev -type s
echo "Test[9]"
test_suite /dev -type d -o -type f
echo "Test[10]"
test_suite /dev -type d -o -type regerger
echo "Test[11]"
test_suite /dev -type reger -o -type d
echo "Test[12]"
test_suite /dev -type d -a -type f

echo "Name"

echo "Test[1]"
test_suite -name "evaluation*"
echo "Test[2]"
test_suite -name
echo "Test[3]"
test_suite -name "*.c"
echo "Test[4]"
test_suite -name wefewfewf

echo "Newer"

echo "Test[1]"
test_suite -newer
echo "Test[2]"
test_suite -newer src
echo "Test[3]"
test_suite -newer ewfgewf
echo "Test[4]"
test_suite src tests Makefile -newer src
echo "Test[5]"
test_suite src tests Makefile -newer tests
echo "Test[6]"
test_suite src tests Makefile -newer Makefile
echo "Test[7]"
test_suite . -newer Makefile
echo "Test[8]"
test_suite . -newer src/evaluation.c
echo "Test[9]"
test_suite . -newer rethgrehg
echo "Test[10]"
test_suite -H -newer Makefile
echo "Test[11]"
test_suite -L -newer Makefile
echo "Test[12]"
test_suite -H -L -newer Makefile

echo "Print"

echo "Test[1]"
test_suite
echo "Test[2]"
test_suite -print -print
echo "Test[3]"
test_suite -print -print -print
echo "Test[4]"
test_suite -print -o -print -print
echo "Test[5]"
test_suite -print -print -o -print
echo "Test[6]"
test_suite src -print
echo "Test[7]"
test_suite src -print -a -newer src
echo "Test[8]"
test_suite src -print -o -newer src

echo "Options"

echo "Test[1]"
test_suite -L
echo "Test[2]"
test_suite -H -newer efwef
echo "Test[3]"
test_suite -P
echo "Test[4]"
test_suite -H -P -newer Makefile -o -name evaluation.c
echo "Test[5]"
test_suite -P -H -print -newer Makefile
echo "Test[6]"
test_suite -P -L src
echo "Test[7]"
test_suite -L -P tests src
echo "Test[8]"
test_suite -L -H tests src -newer Makefile
echo "Test[9]"
test_suite -H -L
echo "Test[9]"
test_suite -Hregerg
echo "Test[9]"
test_suite -xxxerg

echo "Options Depth"
test_suite_depth -L
echo "Test[1]"
test_suite_depth -H -newer efwef
echo "Test[2]"
test_suite_depth -P
echo "Test[3]"
test_suite_depth -H -P -newer Makefile -o -name evaluation.c
echo "Test[4]"
test_suite_depth -P -H -print -newer Makefile
echo "Test[5]"
test_suite_depth -P -L src
echo "Test[6]"
test_suite_depth -L -P tests src
echo "Test[7]"
test_suite_depth -L -H tests src -newer Makefile
echo "Test[8]"
test_suite_depth -H -L
echo "Test[9]"
test_suite_depth -Hregerg
echo "Test[10]"
test_suite_depth -xxxerg

echo "Not"

echo "Test[1]"
test_suite \! -name "*.c"
echo "Test[2]"
test_suite \! \! -name "*.c"
echo "Test[3]"
test_suite \! \!
echo "Test[4]"
test_suite \!
echo "Test[5]"
test_suite \! -name "*.c"
echo "Test[6]"
test_suite \! -name "*.c"

echo "Parentheses"

echo "Test[1]"
test_suite \( \)
echo "Test[2]"
test_suite \( -name -src \)
echo "Test[3]"
test_suite \( -name -src \( -name -o -name tests \)
echo "Test[4]"
test_suite \! \( -name -src -o -name tests \)
echo "Test[5]"
test_suite \( \! -name -src -o -name tests \)
echo "Test[6]"
test_suite \( -name -src \) -o -name tests
echo "Test[7]"
test_suite \( -name -src -o -name tests
echo "Test[8]"
test_suite  -name -src \) -o -name tests

echo "Perm"

echo "Test[1]"
test_suite -perm 644
echo "Test[2]"
test_suite -perm -640
echo "Test[3]"
test_suite -perm -102
echo "Test[4]"
test_suite -perm 64
echo "Test[5]"
test_suite -perm /14
echo "Test[6]"
test_suite -perm -64
echo "Test[7]"
test_suite -perm /004
echo "Test[8]"
test_suite -perm 004
echo "Test[9]"
test_suite -perm 0049
echo "Test[10]"
test_suite -perm 1048
echo "Test[11]"
test_suite -perm /000001

echo "User"

echo "Test[1]"
test_suite -user marc.gallant
echo "Test[2]"
test_suite -user marc
echo "Test[3]"
test_suite -user

echo "Group"

echo "Test[1]"
test_suite -group students
echo "Test[2]"
test_suite -group swefwefwef
echo "Test[3]"
test_suite -group

echo "Priority"

echo "Test[1]"
test_suite \( \! -name "*.c" -o \! -name src \)
echo "Test[2]"
test_suite \( -name "*.c" -a -name "evaluation.?" \)
echo "Test[3]"
test_suite -name "*.c" -a \! \( -name "evaluation.?" \)
echo "Test[4]"
test_suite -name "*.c" -o \! \( -name "evaluation.?" \)
echo "Test[5]"
test_suite \( \! -name "*.c" -a \! -name src \)
echo "Test[6]"
test_suite \! \( \! -name "*.c" -o \! -name src \)
echo "Test[7]"
test_suite foo/* \( -name bar -o -name baz \) -newer foo/bar
echo "Test[8]"
test_suite foo/* \! \( -name bar -o -name baz \) -newer foo/bar
echo "Test[9]"
test_suite foo/* \! \( -name bar -o -name baz \) -a -newer foo/bar
echo "Test[10]"
test_suite foo/* \! \( -name bar \) -newer Makefile
echo "Test[11]"
test_suite foo/* \! \( -name bar \) -o -newer Makefile
echo "Test[12]"
test_suite foo/* \! \( -name bar \) -a -newer Makefile
