###########################################################################
##                                                                       ##
## Project: Transition detector.                                         ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: February 7, 2016.                                               ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $^ -o $@
MAKEFILE_DEPEND = .depend

.cc.o:
	c++ -g -Wall -c $< -o $@

.cpp.o:
	c++ -g -Wall -std=c++11 -c $< -o $@

TARGETS = 
TARGETS += test_detectors

all:    $(TARGETS)

depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_detectors: detectors.o readings.o test_detectors.o timeval_utils.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
