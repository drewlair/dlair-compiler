#!/bin/sh
counter=0
for testfile in test/resolver/good*.bminor
do
	if ./exec/bminor --resolve $testfile
	then
        counter=$((counter+1))
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in test/resolver/bad*.bminor 
do
    if ./exec/bminor --resolve $testfile
    then
        echo "$testfile success (FAILURE)"
    else
        counter=$((counter+1))
        echo "$testfile failure (as expected)"
    fi
done
echo "Passed $counter/29 Test Cases"

