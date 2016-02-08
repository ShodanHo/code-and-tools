CFLAGS=-Wall -ggdb -I ~/workspace/org.eclipse.mosquitto/lib -I ~/workspace/org.eclipse.mosquitto/lib/cpp
LDFLAGS= 

TARGETS =
TARGETS += mqtt_timed_publisher

LIBRARIES =
LIBRARIES += ~/workspace/org.eclipse.mosquitto/lib/cpp/libmosquittopp.so
LIBRARIES += ~/workspace/org.eclipse.mosquitto/lib/libmosquitto.so

.PHONY: all clean

all : $(TARGETS)

mqtt_timed_publisher : mqtt_timed_publisher.o $(LIBRARIES)
	${CXX} $^ -o $@ ${LDFLAGS}

mqtt_timed_publisher.o : mqtt_timed_publisher.cpp
	${CXX} -c $^ -o $@ ${CFLAGS}

clean : 
	-rm -f *.o ${TARGETS}
