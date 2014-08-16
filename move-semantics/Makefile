###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date:                                                                 ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $^ -o $@
MAKEFILE_DEPEND = .depend

.cc.o:
	c++ -std=c++11 -g -Wall -c $< -o $@

.cpp.o:
	c++ -std=c++11 -g -Wall -c $< -o $@

TARGETS = 
TARGETS += test_move_semantics

all:	$(TARGETS)

depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_move_semantics: a.o func.o test_move_semantics.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
