#!/bin/bash

# Should delete underscores in the beginnings of function names for all .asm files in current folder
# For now, just deletes underscores. We don't need them, right?
sed -i 's/_//' *.asm
