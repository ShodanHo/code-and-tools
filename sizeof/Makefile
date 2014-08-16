###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: November 15, 2013.                                              ##
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

TARGETS = test_sizeof

all:	$(TARGETS)


depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_sizeof: test_sizeof.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
