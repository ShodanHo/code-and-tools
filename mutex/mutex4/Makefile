###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: March 25, 2016.                                                 ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

CPP_COMPILER_DIRECTIVES =
CPP_COMPILER_DIRECTIVES += -std=c++11
CPP_COMPILER_DIRECTIVES += -g
CPP_COMPILER_DIRECTIVES += -Wall

LINKER_DIRECTIVES =
LINKER_DIRECTIVES += -lgtest
LINKER_DIRECTIVES += -lpthread

LINK=c++ $(LINKER_DIRECTIVES) $^ -o $@

.cpp.o:
	c++ $(CPP_COMPILER_DIRECTIVES) -c $< -o $@

TARGETS =
TARGETS += mutexedConditionedListsTest

all:	$(TARGETS)

doc:

depend:
	c++ $(CPP_COMPILER_DIRECTIVES) -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

mutexedConditionedListsTest: mutexedConditionedListsTest.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
