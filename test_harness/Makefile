###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: September 26, 2013.                                             ##
##                                                                       ##
###########################################################################

##
## $Id: Makefile,v 1.3 2013/09/28 01:39:12 knoppix Exp $
##
## $Log: Makefile,v $
## Revision 1.3  2013/09/28 01:39:12  knoppix
## Removed debug compile and pthreads library.
##
## Revision 1.2  2013/09/28 01:22:12  knoppix
## clean target removes documentation directories (html and latex)
##
## Revision 1.1  2013/09/27 16:28:28  knoppix
## Makefile
##

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cpp.o:
	c++ -Wall -c $< -o $@

TARGETS =
TARGETS += test_test_harness

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf *.o $(TARGETS) $(MAKEFILE_DEPEND) html latex

doc:
	doxygen -g
	doxygen

test_test_harness: test_test_harness.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
