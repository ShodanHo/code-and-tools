###########################################################################
##                                                                       ##
## Project: Boost testing.                                               ##
##                                                                       ##
## Description: Build file for project.                                  ##
##                                                                       ##
## Date: November 1, 2013.                                               ##
##                                                                       ##
###########################################################################

##
## $Id: Makefile,v 1.1 2013/11/02 16:33:08 knoppix Exp $
##
## $Log: Makefile,v $
## Revision 1.1  2013/11/02 16:33:08  knoppix
## Original
##

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

#	c++ -g -Wall -I$(BOOST_DIR) -c $< -o $@

TARGETS = 
TARGETS += test1
TARGETS += test2

all:	$(TARGETS)

depend:
	gcc -MM $(SOURCES) > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test1: test1.o
	$(LINK) -lboost_unit_test_framework

test2: test2.o
	$(LINK) -lboost_unit_test_framework

-include $(MAKEFILE_DEPEND)

## End of file
