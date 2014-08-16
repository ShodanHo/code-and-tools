TARGETS =
TARGETS += test_input_iterator
TARGETS += test_myiterator

CC = gcc
C++ = gcc
CFLAGS = -Wall
C++FLAGS = -Wall

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.cc
	$(C++) -c $(C++FLAGS) $< -o $@

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS) .depend

depend:
	$(C++) -MM *.cc > .depend

test_input_iterator: test_input_iterator.o
	$(C++) $(C++FLAGS) -lstdc++ $< -o $@

test_myiterator: test_myiterator.o
	$(C++) $(C++FLAGS) -lstdc++ $< -o $@

-include .depend
