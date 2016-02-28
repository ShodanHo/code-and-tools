###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: February 28, 2016.                                              ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cpp.o:
	c++ -std=c++11 -g -Wall -c $< -o $@

TARGETS =
TARGETS += middleC

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

middleC: middleC.o
	$(LINK) -lpthread

-include $(MAKEFILE_DEPEND)

## End of file
