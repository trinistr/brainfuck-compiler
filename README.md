Brainfuck compiler
=================
Ever wanted to compile your cool Brainfuck programs directly into x86 assembly?
Maybe not, but now you can!

Assembly code is compatible with NASM.

Features
--------
* Arbitrary memory cell count (may be subject to memory limits)
* Selectable memory cell width - 1, 2, or 4 bytes
* Possibility to call program as a function
* Return values

Compiling executable
--------------------
	make
That's all for now, folks!

Typical usage example
---------------------
	bfcompile hello.bf -w 4 -o helloworld.asm
	nasm -f elf32 helloworld.asm
	gcc -m32 helloworld.o -o helloworld
	./helloworld

Other things included
---------------------
* scripts/ directory contains some command-line scripts which can be useful
  bfcompile.bat and bfcompile.sh will compile and run your Brainfuck program
* tests/ contains some exemplary programs to test functionality of the compiler

