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

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $< -o $@
MAKEFILE_DEPEND = .depend

.cc.o:
	c++ -std=c++11 -g -Wall -c $< -o $@

.cpp.o:
	c++ -std=c++11 -g -Wall -c $< -o $@

TARGETS = test_operators test_structs

all:	$(TARGETS)


depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_operators: test_operators.o
	$(LINK)

test_structs: test_structs.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
