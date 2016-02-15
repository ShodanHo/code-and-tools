###########################################################################
##                                                                       ##
## Project: Test the STL.                                                ##
##                                                                       ##
## Description: Tests the Standard Template Library.                     ##
##                                                                       ##
## Date: February 20, 2000.                                              ##
##                                                                       ##
###########################################################################

##
## $Id: Makefile,v 1.3 2013/09/21 19:00:27 knoppix Exp $
##
## $Log: Makefile,v $
## Revision 1.3  2013/09/21 19:00:27  knoppix
## Added .depend symbol
##
## Revision 1.2  2013/09/21 18:58:15  knoppix
## Removed included Makefile
##
## Revision 1.1  2013/09/21 18:01:29  knoppix
## Original
##


.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=gcc $< -o $@ -lstdc++

.cpp.o:
	gcc -g -Wall -c $< -o $@

TARGETS =
TARGETS += test_algorithm
TARGETS += test_for_each

all:	$(TARGETS)

depend:
	gcc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_for_each: test_for_each.o
	$(LINK)

test_algorithm: test_algorithm.o
	$(LINK)

test_list: test_list.o
	$(LINK)

test_map: test_map.o
	$(LINK)

test_multimap: test_multimap.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
