###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: September 26, 2013.                                             ##
##                                                                       ##
###########################################################################

##
## $Id: Makefile,v 1.1 2013/09/26 16:12:57 knoppix Exp $
##
## $Log: Makefile,v $
## Revision 1.1  2013/09/26 16:12:57  knoppix
## Original
##

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS =
TARGETS += maxtemplate

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

maxtemplate: maxtemplate.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
