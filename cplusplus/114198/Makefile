###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 12, 2013.                                               ##
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
	c++ -Wall -c $< -o $@

TARGETS =
TARGETS += exponential

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf *.o $(TARGETS) $(MAKEFILE_DEPEND) html latex

doc:
	doxygen -g
	doxygen

exponential: exponential.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
