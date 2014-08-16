###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: November 20, 2013.                                              ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $< -o $@
MAKEFILE_DEPEND = .depend

.cc.o:
	c++ -g -Wall -c $< -o $@

.cpp.o:
	c++ -g -Wall -c $< -o $@

TARGETS = 
TARGETS += question1
TARGETS += question1-2
TARGETS += question1-3
TARGETS += question1-4
TARGETS += question1-5
TARGETS += question2
TARGETS += question3
TARGETS += question4
TARGETS += question5
TARGETS += question6
TARGETS += question7
TARGETS += question8
TARGETS += question9
TARGETS += question10
TARGETS += question11

all:	$(TARGETS)

depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

question1: question1.o
	$(LINK)

question1-2: question1-2.o
	$(LINK)

question1-3: question1-3.o
	$(LINK)

question1-4: question1-4.o
	$(LINK)

question1-5: question1-5.o
	$(LINK)

question2: question2.o
	$(LINK)

question3: question3.o
	$(LINK)

question4: question4.o
	$(LINK)

question5: question5.o
	$(LINK)

question6: question6.o
	$(LINK)

question7: question7.o
	$(LINK)

question8: question8.o
	$(LINK)

question9: question9.o
	$(LINK)

question10: question10.o
	$(LINK)

question11: question11.o
	$(LINK)

question12: question12.o
	$(LINK)

question13: question13.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
