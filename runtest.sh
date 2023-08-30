#!/bin/sh

for testfile in test/encode/good*.bminor
do
	if ./bminor --encode $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in test/encode/bad*.bminor
do
	if ./bminor --encode $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done
