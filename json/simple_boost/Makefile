###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: February 18, 2016.                                              ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $^ -o $@

.cpp.o:
	c++ -std=c++11 -g -Wall -c $< -o $@

TARGETS =
TARGETS += test_json

all:	$(TARGETS)


depend:
	cc -std=c++11 -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_json: test_json.o
	$(LINK) -lpthread

-include $(MAKEFILE_DEPEND)

## End of file
