###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 14, 2013.                                               ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y .gcov

MAKEFILE_DEPEND = .depend
COVERAGE_DIR = code_coverage

C_LINK=cc $< -o $@
CPP_LINK=c++ $< -o $@

.c.o:
	cc -g -Wall -c $< -o $@

.cpp.o:
	c++ -g -Wall -std=c++11 -c $< -o $@

.c.gcov:
	cc -fprofile-arcs -ftest-coverage $<
	./a.out

TARGETS =
TARGETS += cursor_list
TARGETS += dice_probability_c
TARGETS += dice_probability_cpp
TARGETS += accumulation_c
TARGETS += accumulation_cpp
TARGETS += days_typedef
TARGETS += module1_slide57
TARGETS += point
TARGETS += root_find_c
TARGETS += root_find_cpp
TARGETS += scope
TARGETS += slist

GCOV =
GCOV += dice_probability_c.c.gcov
GCOV += dice_probability_cpp.cpp.gcov
GCOV += accumulation_c.c.gcov
GCOV += accumulation_cpp.cpp.gcov
GCOV += root_find_c.c.gcov
GCOV += root_find_cpp.cpp.gcov

all:	$(TARGETS)

depend:
	cc -MM *.c *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf $(COVERAGE_DIR)
	rm -f *.o a.out *.gcov *.gcda *.gcno
	rm -f $(TARGETS) $(MAKEFILE_DEPEND)

coverage: $(GCOV)
	rm -rf $(COVERAGE_DIR)
	mkdir -p $(COVERAGE_DIR)
	# lcov --directory . --zerocounters
	~/lcov-1.10/bin/lcov --directory . --capture --output-file $(COVERAGE_DIR)/stap.info
	~/lcov-1.10/bin/genhtml -o $(COVERAGE_DIR) $(COVERAGE_DIR)/stap.info

cursor_list: cursor_list.o
	$(CPP_LINK)

days_typedef: days_typedef.o
	$(CPP_LINK)

dice_probability_c.c.gcov: dice_probability_c.c
	cc -fprofile-arcs -ftest-coverage $<
	echo 10000000 | ./a.out
	rm -f a.out
	gcov $<

dice_probability_cpp.cpp.gcov: dice_probability_cpp.cpp
	c++ -fprofile-arcs -ftest-coverage $<
	echo 10000000 | ./a.out
	rm -f a.out
	gcov $<

accumulation_c.c.gcov: accumulation_c.c
	cc -fprofile-arcs -ftest-coverage $<
	./a.out
	rm -f a.out
	gcov $<

accumulation_cpp.cpp.gcov: accumulation_cpp.cpp
	c++ -std=c++11 -fprofile-arcs -ftest-coverage $<
	./a.out
	rm -f a.out
	gcov $<

root_find_c.c.gcov: root_find_c.c
	cc -fprofile-arcs -ftest-coverage $< -lm
	./a.out
	rm -f a.out
	gcov $<

root_find_cpp.cpp.gcov: root_find_cpp.cpp
	c++ -std=c++11 -fprofile-arcs -ftest-coverage $<
	./a.out
	rm -f a.out
	gcov $<

dice_probability_c: dice_probability_c.o
	$(C_LINK)

dice_probability_cpp: dice_probability_cpp.o
	$(CPP_LINK)

accumulation_c: accumulation_c.o
	$(C_LINK)

accumulation_cpp: accumulation_cpp.o
	$(CPP_LINK)

module1_slide57: module1_slide57.o
	$(CPP_LINK)

point: point.o
	$(CPP_LINK)

root_find_c: root_find_c.o
	$(C_LINK) -lm

root_find_cpp: root_find_cpp.o
	$(CPP_LINK)

scope: scope.o
	$(CPP_LINK)

slist: slist.o
	$(CPP_LINK)

-include $(MAKEFILE_DEPEND)

## End of file
