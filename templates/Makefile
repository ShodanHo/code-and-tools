###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 21, 2013.                                               ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

LINK=c++ $^ -o $@

.cpp.o:
	c++ -g -Wall -c -std=c++11 $< -o $@

TARGETS =
TARGETS += test_implicit
TARGETS += test_templates
TARGETS += test_variables

all:	$(TARGETS)


depend:
	cc -MM *.cc > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_implicit: implicit.o test_implicit.o
	$(LINK)

test_templates: test_templates.o
	$(LINK)

test_variables: test_variables.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
