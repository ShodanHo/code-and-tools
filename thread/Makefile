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
## $Id: Makefile,v 1.1 2003/07/10 13:54:05 stuartf Exp $
##
## $Log: Makefile,v $
## Revision 1.1  2003/07/10 13:54:05  stuartf
## Original
##

include ../../include/Makefile

.SUFFIXES:
.SUFFIXES: .c .cc .o .y

LINK=c++ -lpthread $< -o $@

.cc.o:
	c++ -g -Wall -std=c++0x -c $< -o $@

TARGETS = test_threads

all:	$(TARGETS)


depend:
	cc -std=c++0x -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_threads: test_threads.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
