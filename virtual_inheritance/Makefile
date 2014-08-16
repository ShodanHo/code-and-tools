###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: November 14, 2013.                                              ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $< -o $@
MAKEFILE_DEPEND = .depend

.cc.o:
	c++ -g -Wall -c $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS = test_virtual_inheritance

all:	$(TARGETS)


depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_virtual_inheritance: test_virtual_inheritance.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
