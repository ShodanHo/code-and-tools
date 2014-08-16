###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: November 11, 2013.                                              ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cc.o:
	c++ -g -Wall -c $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS = test_casting

all:	$(TARGETS)


depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_casting: test_casting.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
