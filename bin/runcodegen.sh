#!/bin/sh

counter=0
x=10
for testfile in test/codegen/good*.bminor
do    
    ./exec/bminor --codegen $testfile > $testfile.s
    if gcc -g $testfile.s src/library.c -o bin/out
    then

        ./bin/out > $testfile.out
        if [ "$?" != "10" ]
        then
            echo "$testfile has different return value than expected"
        else
            if diff $testfile.out $testfile.expected
            then
                counter=$((counter+1))
                echo "$testfile success (as expected)"
            else
                echo "$testfile failure (INCORRECT)"
            fi
        fi

    else
        echo "$testfile segfault (INCORRECT)"
    fi
    

done
echo "Passed $counter test cases"



