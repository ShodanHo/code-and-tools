###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 15, 2013.                                               ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

LINK=c++ $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS =
TARGETS += test0
TARGETS += test1
TARGETS += test2
TARGETS += test3
TARGETS += test4
TARGETS += test_rule_of_three

all:	$(TARGETS)

depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test0: test0.o
	$(LINK)

test1: test1.o
	$(LINK)

test2: test2.o
	$(LINK)

test3: test3.o
	$(LINK)

test4: test4.o
	$(LINK)

test_rule_of_three: test_rule_of_three.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
