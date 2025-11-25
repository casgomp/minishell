#!/bin/bash

echo No of params: $#

for (( i=0; i<=$#; ++i)); do
	printf "arg %d: " $i
	echo  "${@:$i:1}"
done
