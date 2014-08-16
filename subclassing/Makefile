###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 23, 2013.                                               ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

LINK=c++ $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS = test_virtual_subclassing

all:	$(TARGETS)


depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_virtual_subclassing: test_virtual_subclassing.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
