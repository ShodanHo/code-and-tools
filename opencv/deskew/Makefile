###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: April 5, 2016.                                                  ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

CPP_COMPILER_DIRECTIVES =
CPP_COMPILER_DIRECTIVES += -std=c++11
CPP_COMPILER_DIRECTIVES += -g
CPP_COMPILER_DIRECTIVES += -Wall
CPP_COMPILER_DIRECTIVES += -I ../templates
CPP_COMPILER_DIRECTIVES += -I ../common

LINKER_DIRECTIVES =
//LINKER_DIRECTIVES += -lgtest
LINKER_DIRECTIVES += $(shell pkg-config --libs --static opencv)

LINK=c++ $(LINKER_DIRECTIVES) $^ -o $@

.cpp.o:
	c++ $(CPP_COMPILER_DIRECTIVES) -c $< -o $@

TARGETS =
TARGETS += deskew

all:	$(TARGETS)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

depend:
	c++ $(CPP_COMPILER_DIRECTIVES) -MM *.cpp > $(MAKEFILE_DEPEND)

deskew: deskew.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
