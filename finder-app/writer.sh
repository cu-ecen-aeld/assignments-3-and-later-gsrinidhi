#!/bin/bash
nargs=$#
if [ ${nargs} -ne 2 ]
then
	echo "nargs not equal to 2"
	exit 1
fi
writefile=$1
writestr=$2
var1=$(basename $1)
var2=${writefile%${var1}}
ls ${var2} > /dev/null
if [ $? -ne 0 ]
then
	mkdir ${var2} -p
fi
touch ${writefile}
echo ${writestr} > ${writefile}
