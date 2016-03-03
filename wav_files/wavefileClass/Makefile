###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: March 2, 2016.                                                  ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend
COMPILE_DIRECTIVES =
COMPILE_DIRECTIVES += -std=c++11
COMPILE_DIRECTIVES += -g
COMPILE_DIRECTIVES += -Wall
COMPILE_DIRECTIVES += -I ../../templates
COMPILE_DIRECTIVES += -I ../../common

LINK=c++ $^ -o $@

.cpp.o:
	c++ $(COMPILE_DIRECTIVES) -c $< -o $@

TARGETS =
TARGETS += test_waveFile

all:	$(TARGETS)


depend:
	c++ $(COMPILE_DIRECTIVES) -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_waveFile: test_waveFile.o waveFile.o
	$(LINK) -lgtest

-include $(MAKEFILE_DEPEND)

## End of file
