###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: November 21, 2013                                               ##
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
	c++ -std=c++11 -g -Wall -c $< -o $@

.cpp.o:
	c++ -std=c++11 -g -Wall -c $< -o $@

TARGETS = 
TARGETS += test_capitalisation

all:	$(TARGETS)

depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_capitalisation: test_capitalisation.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
