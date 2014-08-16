###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 12, 2013.                                               ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$


.SUFFIXES:
.SUFFIXES: .c .cc .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cc.o:
	c++ -Wall -c $< -o $@

TARGETS =
TARGETS += convertBinary

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf *.o $(TARGETS) $(MAKEFILE_DEPEND) html latex

doc:
	doxygen -g
	doxygen

convertBinary: convertBinary.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
