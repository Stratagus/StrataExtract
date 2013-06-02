#!/bin/bash

UPPER="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
LOWER="abcdefghijklmnopqrstuvwxyz"

filename=$1
tempfile=${filename}.tmp
xmlfile=${filename}.xml

touch $tempfile
touch $xmlfile

# Clean up the input
cat $filename | cut -d \, -f 3-4 | cut -d \" -f 2,4 --output-delimiter=, | sed -re 's/\ /\+/g' > $tempfile

# Create a new XML document
echo -e '<?xml version="1.0"?>\n<xml>\n</xml>' > $xmlfile

# Determines a file's type from its extension
setFileType() {

	ext=$(echo "$ext" | sed -re 'y/"$UPPER"/"$LOWER"/') # Convert uppercase to lowercase
	case "$ext" in
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

	# Convert C comments into XML comments
	then
		echo "Comment $value"
		head -n -1 $xmlfile > ${xmlfile}.tmp  # Grab all but the last line of $xmlfile and output it to new file $xmlfile.tmp
		# echo "" >> $xmlfile.tmp
		value=$(echo $value | sed -re 's/\+/\ /g' | sed -re 's/\/\/\ //')	# Replace '+' with ' ', replace '// ' with nothing
		echo "<!-- $value -->" >> ${xmlfile}.tmp 							# Add the comment to the document

		#  We don't know how many values the input file has, so we append '</xml>' every time. If there's still another value,
		#+ this ending tag gets cut off above where we call 'head -n -1'. This ensures that we only have one closing '</xml>' tag in the document
		echo "</xml>" >> ${xmlfile}.tmp

		diff $xmlfile ${xmlfile}.tmp | patch $xmlfile - # Add the new tag to the XML document
		rm ${xmlfile}.tmp

	# Parse info about files and create XML tags for them
	else
		path=$(echo $value | cut -d \, -f 2)								# Use ',' as the delimiter, grab the second field
		destination=$(echo $value | cut -d \, -f 1 | sed -re 's/\+/\ /g')	# USe ',' as the delimiter, grab the first field, replace '+' with ' '
		ext=$(echo "$path" | cut -d \. -f 2)								# USe '.' as the delimiter, grab the second field

		# If file has an extension, figure out its type
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

		xmlstarlet ed -L -s /xml -t elem -n "$filetype"TMP -v "" \			# Create new XML tag with the name of the filetype (e.g. 'image' for .pcx files). Add 'TMP' to uniquely identify our new tag while we work on it
		-i //"$filetype"TMP -t attr -n "name" -v "" \						# Add an attribute called 'name', leave it blank
		-i //"$filetype"TMP -t attr -n "path" -v "$path" \					# Add an attribute called 'path', set it equal to the file's path (e.g. 'path/to/file.ext')
		-i //"$filetype"TMP -t attr -n "destination" -v "$destination" \	# Add an attribute called 'destination', set it equal to the location where the file will be extracted to
		-r //"$filetype"TMP -v $filetype \									# We're done, remove the 'TMP' tag
		$xmlfile
	fi
done