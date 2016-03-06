###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: March 3, 2016.                                                  ##
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
LINKER_DIRECTIVES += -lgtest

LINK=c++ $(LINKER_DIRECTIVES) $^ -o $@

.cpp.o:
	c++ $(CPP_COMPILER_DIRECTIVES) -c $< -o $@

TARGETS =
TARGETS += test_sample_generator

all:	$(TARGETS)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

depend:
	c++ $(CPP_COMPILER_DIRECTIVES) -MM *.cpp > $(MAKEFILE_DEPEND)

test_sample_generator: test_sample_generator.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
