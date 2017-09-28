MAIN = bfcompile

CC = gcc
CXX = g++

CFLAGS = -Wall -g
INCLUDES = -I.
LDFLAGS = -L.
LDLIBS =

SRCS = bfcompile.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: clean

all:	$(MAIN)

$(MAIN):	$(OBJS)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) *.o *~ $(MAIN)
