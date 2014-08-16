###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date:                                                                 ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $< -o $@
MAKEFILE_DEPEND = .depend

.cc.o:
	c++ -g -Wall -c $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS = 
TARGETS += test1-4
TARGETS += test2-1
TARGETS += test2-4

all:	$(TARGETS)

depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test1-4: test1-4.o
	$(LINK)

test2-1: test2-1.o
	$(LINK)

test2-4: test2-4.o
	$(LINK)

test2: test2.o
	$(LINK)

test3: test3.o
	$(LINK)

test4: test4.o
	$(LINK)

test5: test5.o
	$(LINK)

test6: test6.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
