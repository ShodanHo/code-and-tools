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
## $Id: Makefile,v 1.1 2013/09/25 00:24:53 knoppix Exp $
##
## $Log: Makefile,v $
## Revision 1.1  2013/09/25 00:24:53  knoppix
## Original
##

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS =
TARGETS += bitcount

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

bitcount: bitcount.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
