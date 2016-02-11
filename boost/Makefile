###########################################################################
##                                                                       ##
## Project: Boost testing.                                               ##
##                                                                       ##
## Description: Build file for project.                                  ##
##                                                                       ##
## Date: December 24, 2005.                                              ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

include ../include/Makefile

.SUFFIXES:
.SUFFIXES: .c .cc .o .y

LINK=gcc $< -o $@ -lstdc++
BOOST_DIR=/tmp/boost_1_33

.cc.o:
	gcc -g -Wall -I$(BOOST_DIR) -c $< -o $@

TARGETS = test_format test_lexical_cast

SOURCES = test_lexical_cast.cc

all:	$(TARGETS)

depend:
	gcc -MM $(SOURCES) > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

-include $(MAKEFILE_DEPEND)

test_format: test_format.o
	$(LINK) -lpopt

test_lexical_cast: test_lexical_cast.o
	$(LINK) -lpopt

## End of file
