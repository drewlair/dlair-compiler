#!/bin/sh

counter=0
for testfile in test/typechecker/good*.bminor
do
	if ./exec/bminor --typecheck $testfile > $testfile.out
	then
        counter=$((counter+1))
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in test/typechecker/bad*.bminor
do
	if ./exec/bminor --typecheck $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
        counter=$((counter+1))
		echo "$testfile failure (as expected)"
	fi
done

echo "Passed $counter test cases"