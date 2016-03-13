###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: March 13, 2016.                                                 ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

CPP_COMPILER_DIRECTIVES =
CPP_COMPILER_DIRECTIVES += -std=c++11
CPP_COMPILER_DIRECTIVES += -g
CPP_COMPILER_DIRECTIVES += -Wall
CPP_COMPILER_DIRECTIVES += -I ../../templates
CPP_COMPILER_DIRECTIVES += -I ../../common
CPP_COMPILER_DIRECTIVES += -I ../headers

LINKER_DIRECTIVES =
LINKER_DIRECTIVES += -lgtest

LINK=c++ $(LINKER_DIRECTIVES) $^ -o $@
ARCHIVE=ar cr $@ $^

.cpp.o:
	c++ $(CPP_COMPILER_DIRECTIVES) -c $< -o $@

TARGETS =
TARGETS += ../lib/libprebuilt.a

all:	$(TARGETS)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

depend:
	c++ $(CPP_COMPILER_DIRECTIVES) -MM *.cpp > $(MAKEFILE_DEPEND)

../lib/libprebuilt.a: prebuilt.o
	$(ARCHIVE)

-include $(MAKEFILE_DEPEND)

## End of file
