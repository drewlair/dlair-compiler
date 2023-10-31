#!/bin/sh

for testfile in test/printer/good*.bminor
do
	if ./exec/bminor --print $testfile
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done
