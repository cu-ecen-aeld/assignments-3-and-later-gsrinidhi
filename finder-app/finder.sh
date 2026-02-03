#!/bin/bash
nargs=$#
if [ ${nargs} -ne 2 ]
then
	echo "Wrong number of arguments passed"
	exit 1
fi
filesdir=$1
searchstr=$2
cd $filesdir
p=$?
if [ ${p} -ne 0 ]
then
	echo "Directory does not exit"
	exit 1
fi
var1=$(ls -Rp | grep -v / | wc -l)
var2=$(ls -lR | grep '^d' | wc -l)
x=$((var1 - var2 - 1))
y=$(grep -r ${searchstr} * | wc -l)
echo "The number of files are "${x}" and the number of matching lines are "${y}
