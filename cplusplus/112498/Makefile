###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: September 30, 2013.                                             ##
##                                                                       ##
###########################################################################

##
## $Id: Makefile,v 1.1 2013/10/04 17:31:19 knoppix Exp $
##
## $Log: Makefile,v $
## Revision 1.1  2013/10/04 17:31:19  knoppix
## Original
##

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cpp.o:
	c++ -Wall -c $< -o $@

TARGETS =
TARGETS += calculator

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf *.o $(TARGETS) $(MAKEFILE_DEPEND) html latex

doc:
	doxygen -g
	doxygen

calculator: calculator.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
