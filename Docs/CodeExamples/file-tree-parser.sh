#!/bin/bash

dir=$1

GetFileTree()
{
	local input

	input=$1

	ls -Rp1 $input
}

RemoveDirectories()
{
	local input
	local output
	
	input=$1

	while read -r output; do
		echo $output | sed -e 's/.*\/$//' | sed '/^$/ d' | sed -e 's/^\./\n\./' | sed -e 's/\:/\//' >> filetree.tmp2
	done < $input

	output=$(cat filetree.tmp2)

	echo $output

	rm filetree.tmp2
}

FilterCharacters()
{
	local input

	input=$1

	cat $input | sed -e 's/\.\///'
}

(
	cd $dir

	GetFileTree . > filetree.tmp

	directory=""

	for i in $(RemoveDirectories filetree.tmp)
	do
		if echo $i | grep '/' > /dev/null
		then
			directory=${i}
		else
			echo ${directory}${i} >> filetree.tmp3
		fi
	done

	FilterCharacters filetree.tmp3 > filetree.tmp4

	rm filetree.tmp
	rm filetree.tmp3
	mv filetree.tmp4 filetree.tmp
)