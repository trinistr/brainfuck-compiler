../bfcompile %1.bf -w4 -z -v -t windows
nasm -f win32 %1.asm
del %1.asm
gcc %1.obj -o %1
del %1.obj
echo
echo Running %1:
%1

