###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 12, 2013.                                               ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$


.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=c++ $< -o $@

.cpp.o:
	c++ -std=c++11 -Wall -c $< -o $@

TARGETS =
TARGETS += 112148
TARGETS += 115329
TARGETS += 115305
TARGETS += 115268
TARGETS += 115511
TARGETS += 116373
TARGETS += 117643
TARGETS += 118194
TARGETS += 118902
TARGETS += 119450
TARGETS += 120086
TARGETS += 120124
TARGETS += 120428
TARGETS += 121157
TARGETS += 123804
TARGETS += 123975
TARGETS += 145532

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf *.o $(TARGETS) $(MAKEFILE_DEPEND) html latex

doc:
	doxygen -g
	doxygen

112148: 112148.o
	$(LINK)

115329: 115329.o
	$(LINK)

115305: 115305.o
	$(LINK)

115268: 115268.o
	$(LINK)

115511: 115511.o
	$(LINK)

116373: 116373.o
	$(LINK)

117643: 117643.o
	$(LINK)

118194: 118194.o
	$(LINK)

118902: 118902.o
	$(LINK)

119450: 119450.o
	$(LINK)

120086: 120086.o
	$(LINK)

120124: 120124.o
	$(LINK)

120428: 120428.o
	$(LINK)

121157: 121157.o
	$(LINK)

123804: 123804.o
	$(LINK)

123975: 123975.o
	$(LINK)

145532: 145532.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
