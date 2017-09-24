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

# Source files
SRCS = $(LEXERC) $(PARSERC)
LSRCS = bf.l
YSRCS = bf.y
# Generated files
LEXERC = lex.bf.c
PARSERC = bf.tab.c
OBJS = $(SRCS:.c=.o)

all: $(MAIN)

stripped: $(MAIN)
	strip $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -o $(MAIN) $(OBJS) $(LIBPATHS) $(LIBS)
	
$(LEXERC): $(LSCRS) $(PARSERC:.c=.h)
	$(LEX) $(LFLAGS) $(LSRCS)
	
$(PARSERC) $(PARSERC:.c=.h): $(YSRCS)
	$(YACC) -d $(YFLAGS) $(YSRCS)
	
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@
	
.PHONY: clean

clean:
	$(RM) $(MAIN) *.o $(PARSERC:.c=.h) $(LEXERC) $(PARSERC) *~
