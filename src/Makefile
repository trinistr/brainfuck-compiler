# Name definitions
MAIN = bfcompile

# Compilers and stuff
CC = gcc
LEX = flex
YACC = bison

# Flags
CFLAGS = -Wall -ggdb3
INCLUDEPATHS =
LIBPATHS =
LIBS =
LFLAGS = -o $(LEXERC)
YFLAGS = -o $(PARSERC)

# Files
SRCS = $(LEXERC) $(PARSERC)
LSRCS = bf.l
YSRCS = bf.y
OBJS = $(SRCS:.c=.o)

LEXERC = lex.bf.c
PARSERC = bf.tab.c

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -o $(MAIN) $(OBJS) $(LIBPATHS) $(LIBS)
	
$(LEXERC):
	$(LEX) $(LFLAGS) $(LSRCS)
	
$(PARSERC):
	$(YACC) $(YFAGS) $(YSRCS)
	
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@
	
.PHONY: clean

clean:
	$(RM) $(MAIN) *.o $(LEXERC) $(PARSERC) *~
