###########################################################################
##                                                                       ##
## Project: MQTT Publisher.                                              ##
##                                                                       ##
## Description: Build for this directory.                                ##
##                                                                       ##
## Date: February 21, 2016.                                              ##
##                                                                       ##
###########################################################################

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o .y

LINK=c++ $^ -o $@ -l mosquitto -l pthread -l popt
MAKEFILE_DEPEND = .depend
INCLUDES = -I ../../mutex3 -I ../../common

.cc.o:
	c++ $(INCLUDES) -g -Wall -c $< -o $@

.cpp.o:
	c++ $(INCLUDES) -g -Wall -std=c++11 -c $< -o $@

TARGETS = 
TARGETS += test_mqtt_pub_plus_sub

all:    $(TARGETS)

depend:
	c++ $(INCLUDES) -std=c++11 -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -f *.o $(TARGETS) $(MAKEFILE_DEPEND)

test_mqtt_pub_plus_sub: mqtt_publisher.o mqtt_subscriber.o test_mqtt_pub_plus_sub.o 
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file
