###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: March 22, 2003.                                                 ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cc .o .y

LINK=c++ $< -o $@

.cc.o:
	c++ -g -Wall -c $< -o $@

TARGETS = q32

all:	$(TARGETS)


depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

q32: q32.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
