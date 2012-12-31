#!/bin/bash

filename=$1
tempfile=$filename.tmp
xmlfile=$filename.xml

touch $tempfile
touch $xmlfile

cat $filename | cut -d \, -f 3-4 | cut -d \" -f 2,4 --output-delimiter=, | sed -e 's/\ /_/' > $tempfile

exec 3<> $xmlfile
echo -e '<?xml version="1.0"?>\n<xml>\n</xml>' > $xmlfile

for value in $(cat $tempfile)
do
	path=$(echo $value | cut -d \, -f 2)
	destination=$(echo $value | cut -d \, -f 1)

	xmlstarlet ed -L -s /xml -t elem -n audioTMP -v "" \
	-i //audioTMP -t attr -n "name" -v "" \
	-i //audioTMP -t attr -n "path" -v "$path" \
	-i //audioTMP -t attr -n "destination" -v "$destination" \
	-r //audioTMP -v audio \
	$xmlfile
done

