Brainfuck compiler
=================

Ever wanted to compile your cool Brainfuck programs directly into x86 assembly?
Maybe not, but now you can!

Assembly code is compatible with NASM. Depending on memory width, it may be x86 or x86_64.

Features
--------

* Arbitrary memory cell count (may be subject to memory limits)
* Selectable memory cell width - 1, 2, 4 or 8 bytes
* Possibility to call program as a function
* Return values

Compiling executable
--------------------

	cd src
	make
That's all fo now

Typical usage example
---------------------

	bfcompile hello.bf -w 4 -o helloworld.asm
	nasm -f elf32 helloworld.asm
	gcc -m 32 helloworld.o -o helloworld
	./helloworld

