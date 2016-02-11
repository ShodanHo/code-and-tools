###########################################################################
##                                                                       ##
## Project: C++ investigation.                                           ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: October 25, 2013.                                               ##
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
	c++ -g -Wall -c $< -o $@

TARGETS =
TARGETS += test_container_device
TARGETS += test_container_sink
TARGETS += test_container_source
TARGETS += test_dictionary
TARGETS += test_filtering_istream_comments
TARGETS += test_filtering_istream_gunzip
TARGETS += test_filtering_ostream_comments
TARGETS += test_filtering_ostream_gzip
TARGETS += test_stream_buffer_file_sink

all:	$(TARGETS)


depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_container_device: test_container_device.o
	$(LINK)

test_container_sink: test_container_sink.o
	$(LINK)

test_container_source: test_container_source.o
	$(LINK)

test_dictionary: test_dictionary.o
	$(LINK)

test_filtering_istream_comments: test_filtering_istream_comments.o
	$(LINK) -l boost_iostreams

test_filtering_istream_gunzip: test_filtering_istream_gunzip.o
	$(LINK) -l boost_iostreams

test_filtering_ostream_comments: test_filtering_ostream_comments.o
	$(LINK) -l boost_iostreams

test_filtering_ostream_gzip: test_filtering_ostream_gzip.o
	$(LINK) -l boost_iostreams

test_stream_buffer_file_sink: test_stream_buffer_file_sink.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
