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

LINK=c++ $< -o $@

.cc.o:
	c++ -g -Wall -c $< -o $@

TARGETS = test_lexer

all:	$(TARGETS)


depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_lexer: test_lexer.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
