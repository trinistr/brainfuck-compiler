bfcompile %1.txt %*
nasm -f win %1.asm
gcc %1.obj -o %1
%1

