#!/bin/sh

for testfile in test/parser/good*.bminor
do
	if ./exec/bminor --parse $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in test/parser/bad*.bminor
do
	if ./exec/bminor --parse $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done
