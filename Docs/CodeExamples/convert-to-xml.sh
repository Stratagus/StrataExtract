#!/bin/bash

UPPER="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
LOWER="abcdefghijklmnopqrstuvwxyz"

filename=$1
tempfile=$filename.tmp
xmlfile=$filename.xml

touch $tempfile
touch $xmlfile

cat $filename | cut -d \, -f 3-4 | cut -d \" -f 2,4 --output-delimiter=, | sed -re 's/\ /\+/g' > $tempfile

echo -e '<?xml version="1.0"?>\n<xml>\n</xml>' > $xmlfile

setFileType() {

	ext=$(echo "$ext" | sed -re 'y/"$UPPER"/"$LOWER"/')
	case "$ext" in
		EXE)
			echo "file"
			;;
		exe)
			echo "file"
			;;
		mpq)
			echo "file"
			;;
		wav)
			echo "audio"
			;;
		WAV)
			echo "audio"
			;;
		scm)
			echo "map"
			;;
		pcx)
			echo "image"
			;;
		smk)
			echo "video"
			;;
		fnt)
			echo "font"
			;;
		chk)
			echo "campaign"
			;;
		txt)
			echo "text"
			;;
		grp)
			echo "grp"
			;;
		*) echo ""
			;;
	esac;
}

for value in $(cat $tempfile)
do
	if echo $value | grep // | grep -v { > /dev/null
	then
		echo "Comment $value"
		head -n -1 $xmlfile > $xmlfile.tmp
		# echo "" >> $xmlfile.tmp
		value=$(echo $value | sed -re 's/\+/\ /g' | sed -re 's/\/\/\ //')
		echo "<!-- $value -->" >> $xmlfile.tmp
		echo "</xml>" >> $xmlfile.tmp

		diff $xmlfile $xmlfile.tmp | patch $xmlfile -
		rm $xmlfile.tmp
	else
		path=$(echo $value | cut -d \, -f 2)
		destination=$(echo $value | cut -d \, -f 1 | sed -re 's/\+/\ /g')
		ext=$(echo "$path" | cut -d \. -f 2)

		if echo $path | grep '\.' > /dev/null
		then
			filetype=$(setFileType)
		else
			filetype="unknown"
		fi

		echo "Value: $value"
		echo "Destination: $destination"
		echo "Path: $path"
		echo "Extention: $ext"
		echo "Filetype: $filetype"

		echo "adding element"

		xmlstarlet ed -L -s /xml -t elem -n "$filetype"TMP -v "" \
		-i //"$filetype"TMP -t attr -n "name" -v "" \
		-i //"$filetype"TMP -t attr -n "path" -v "$path" \
		-i //"$filetype"TMP -t attr -n "destination" -v "$destination" \
		-r //"$filetype"TMP -v $filetype \
		$xmlfile
	fi
done