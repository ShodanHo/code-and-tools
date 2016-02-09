###########################################################################
##                                                                       ##
## Project: MQTT Publisher.                                              ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: February 9, 2016.                                               ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $^ -o $@ -L ~/workspace/mosquitto-1.4.7/lib -l mosquitto -l pthread
MAKEFILE_DEPEND = .depend
INCLUDES = -I ~/workspace/mosquitto-1.4.7/lib -I ../../mutex3

.cc.o:
	c++ $(INCLUDES) -g -Wall -c $< -o $@

.cpp.o:
	c++ $(INCLUDES) -g -Wall -std=c++11 -c $< -o $@

TARGETS = 
TARGETS += test_mqtt_publisher

all:    $(TARGETS)

depend:
	cc -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_mqtt_publisher: mqtt_publisher.o test_mqtt_publisher.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
