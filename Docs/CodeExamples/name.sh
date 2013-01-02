#!/bin/bash

filetype=0

case "$1" in
	-t)
		filetype=$2
		filename=$3
		search_string=$4
		replace_string=$5
		;;
	
	help)
		echo ""
		echo "Usage ./name [-t] [type] <file> <search> <replace>"
		echo ""
		;;

	*)
		filename=$1
		search_string=$2
		replace_string=$3
		;;
esac

if [ $filetype = 0 ]
then
	filetype="*"

for value in $(cat $filename | grep $search_string | sed -e 's/\ \{2\}//' | sed -e 's/\ /\t/g' | cut -f 3 | cut -d \" -f 2)
do
	n=1
	for i in `seq -w 0 10`
	do
		if echo $value | grep $i > /dev/null
		then
			xmlstarlet ed -u "/xml/$filetype[@path='$value']/@name" -v $replace_string$n $filename | diff $filename - | patch $filename -
		else
			n=`expr $n + 1`
		fi
	done
done
