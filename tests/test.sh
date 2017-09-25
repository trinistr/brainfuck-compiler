#!/bin/bash

# Run this to test compiler on every file in this directory

compiler=../src/bfcompile
for file in *.bf; do
	echo
	echo "$file"
	$compiler "$file"
done
