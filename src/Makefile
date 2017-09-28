# Name definitions
MAIN = ../bfcompile

# Compilers and stuff
CC = gcc
LEX = flex
YACC = bison

# Flags
CFLAGS = -Wall -ggdb3
INCLUDEPATHS =
LIBPATHS =
LIBS =

# Source files
SRCS =

all: $(MAIN)

stripped: $(MAIN)
	strip $(MAIN)

$(MAIN): lex.bf.o bf.tab.o $(SRCS:.c=.o)
	$(CC) $(CFLAGS) $(INCLUDEPATHS) $^ $(LIBPATHS) $(LIBS) -o $(MAIN)
	
lex.%.c: %.l %.tab.h
	$(LEX) -o $@ $<
	
%.tab.c %.tab.h: %.y
	$(YACC) -d $<
	
.PHONY: clean

clean:
	$(RM) $(MAIN) *.o *~ lex.* *.tab.*
