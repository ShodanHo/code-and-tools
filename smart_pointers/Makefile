###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 17, 2013.                                               ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cpp.o:
	c++ -std=c++11 -Wall -c $< -o $@

TARGETS =
TARGETS += test_shared_ptr

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf *.o $(TARGETS) $(MAKEFILE_DEPEND) html latex

doc:
	doxygen -g
	doxygen

test_shared_ptr: test_shared_ptr.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
