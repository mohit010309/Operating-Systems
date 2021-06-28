#!/bin/bash
cmdLine=(Laxmi Mittal College Rupa-ki-Nangal 302006)
echo ${cmdLine}

# Only picks one string
for param in $cmdLine; do
	echo $param
done

# Use index
echo ${cmdLine[2]} ${cmdLine[1]} ${cmdLine[0]}

# Itereate for each array element
echo ${cmdLine[@]}
for param in ${cmdLine[@]}; do
	echo $param
done
