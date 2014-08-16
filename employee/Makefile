###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: March 22, 2003.                                                 ##
##                                                                       ##
###########################################################################

##
## $Id: Makefile,v 1.1 2003/07/10 13:54:05 stuartf Exp $
##
## $Log: Makefile,v $
## Revision 1.1  2003/07/10 13:54:05  stuartf
## Original
##

include ../../include/Makefile

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $^ -o $@

.cc.o:
	c++ -g -Wall -c $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS = employeeTest

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

employeeTest: employeeTest.o employee.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
