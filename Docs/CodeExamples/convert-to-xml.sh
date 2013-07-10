#!/bin/bash

UPPER="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
LOWER="abcdefghijklmnopqrstuvwxyz"

file=$1
xmlfile=${file}.xml

touch $xmlfile

ops=$2

# Create a new XML document
echo -e '<?xml version="1.0"?>\n<xml>\n</xml>' > $xmlfile

# Gets the file extension
GetExtension()
{
	local input
	input=$(echo "$1" | sed -re 'y/"$UPPER"/"$LOWER"/') 	# Convert uppercase to lowercase

	echo $input | grep -o -E '\.[a-z]{3}$' | grep -o -E '[a-z]{3}'	# Search the end of the string for '.xxx' and output only the letters.
}

# Gets the tree depth (how many folders deep a file is)
GetTreeDepth()
{
	local input
	input=$1

	echo "$input" | grep -o / | wc -l						# Search string for '/' and output only the '/'s, one on each line. Then count the number of lines
}

# Determines a file's type from its extension
GetFileType()
{
	local input

	input=$(echo "$1" | sed -re 'y/"$UPPER"/"$LOWER"/') # Convert uppercase to lowercase

	case "$input" in
		exe)
			echo "file"
			;;
		mpq)
			echo "file"
			;;
		wav)
			echo "audio"
			;;
		scm)
			echo "map"
			;;
		grp)
			echo "image"
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
		*) echo "unknown"
			;;
	esac
}

for value in $(cat $file)
do
	path=$value
	destination=$(echo $value | cut -d / -f 1-$(GetTreeDepth $value))
	ext=$(GetExtension $value)

	# If file has an extension, figure out its type
	if echo $path | grep '\.' > /dev/null
	then
		filetype=$(GetFileType $ext)
	else
		filetype="unknown"
	fi

	if [[ $ops == "-v" ]]; then
		echo "Value: $value"
		echo "Destination: $destination"
		echo "Path: $path"
		echo "Extention: $ext"
		echo "Filetype: $filetype"

		echo "adding element"
		echo ""
	fi

	# xmlstarlet ed -L -s /xml -t elem -n "$filetype"TMP -v "" \			# Create new XML tag with the name of the filetype (e.g. 'image' for .pcx files). Add 'TMP' to uniquely identify our new tag while we work on it
	# -i //"$filetype"TMP -t attr -n "name" -v "" \							# Add an attribute called 'name', leave it blank
	# -i //"$filetype"TMP -t attr -n "path" -v "$path" \					# Add an attribute called 'path', set it equal to the file's path (e.g. 'path/to/file.ext')
	# -i //"$filetype"TMP -t attr -n "destination" -v "$destination" \		# Add an attribute called 'destination', set it equal to the location where the file will be extracted to
	# -r //"$filetype"TMP -v $filetype \									# We're done, remove the 'TMP' tag
	# $xmlfile

	xmlstarlet ed -L -s /xml -t elem -n "$filetype"TMP -v "" -i //"$filetype"TMP -t attr -n "name" -v "" -i //"$filetype"TMP -t attr -n "path" -v "$path" -i //"$filetype"TMP -t attr -n "destination" -v "$destination" -r //"$filetype"TMP -v $filetype $xmlfile

done