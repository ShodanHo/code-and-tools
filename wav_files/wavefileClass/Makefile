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

LINK=c++ $^ -o $@

.cpp.o:
	c++ -std=c++11 -g -Wall -c $< -o $@

TARGETS =
TARGETS += test_waveFile

all:	$(TARGETS)


depend:
	c++ -std=c++11 -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_waveFile: test_waveFile.o waveFile.o
	$(LINK) -lgtest

-include $(MAKEFILE_DEPEND)

## End of file
