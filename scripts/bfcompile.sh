#!/bin/bash

if [ ! -f $1 -o -z $1 ]; then
	echo "$1 is not a valid file"
	exit 1
fi

name=$(basename $1 .bf)
../src/bfcompile "$1" -w 4 -z
../scripts/change_names.sh
nasm -f elf32  "$name.asm"
gcc -m32 "$name.o" -o "$name"
./$name
