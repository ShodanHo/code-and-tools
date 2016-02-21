###########################################################################
##                                                                       ##
## Project: MQTT Subscriber.                                             ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: February 11, 2016.                                              ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

#LINK=c++ $^ -o $@ -L ~/workspace/mosquitto-1.4.7/lib -l mosquitto -l pthread
LINK=c++ $^ -o $@ -l mosquitto -l pthread -l popt
MAKEFILE_DEPEND = .depend
#INCLUDES = -I ~/workspace/mosquitto-1.4.7/lib -I ../../mutex3
INCLUDES = -I ../../mutex3 -I ../../common

.cc.o:
	c++ $(INCLUDES) -g -Wall -c $< -o $@

.cpp.o:
	c++ $(INCLUDES) -g -Wall -std=c++11 -c $< -o $@

TARGETS = 
TARGETS += test_mqtt_subscriber

all:    $(TARGETS)

depend:
	c++ $(INCLUDES) -std=c++11 -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_mqtt_subscriber: mqtt_subscriber.o test_mqtt_subscriber.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
