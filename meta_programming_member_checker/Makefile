###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: November 12, 2015.                                              ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=g++ $^ -o $@

.cpp.o:
	g++ -std=c++11 -Wall -Wextra -I. -c $< -o $@

TARGETS =
TARGETS += meta_programming_member_checker

all:	$(TARGETS)

depend:
	g++ -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf *.o $(TARGETS) $(MAKEFILE_DEPEND) html latex

doc:
	doxygen -g
	doxygen

meta_programming_member_checker: meta_programming_member_checker.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file 
