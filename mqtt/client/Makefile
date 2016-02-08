CFLAGS=-Wall -ggdb -I ~/workspace/org.eclipse.mosquitto/lib -I ~/workspace/org.eclipse.mosquitto/lib/cpp
LDFLAGS=-L/home/stuartf/workspace/org.eclipse.mosquitto/lib
LIBRARIES=/home/stuartf/workspace/org.eclipse.mosquitto/lib/libmosquitto.so

TARGETS =
TARGETS += mosquitto_pub

.PHONY: all clean

all : $(TARGETS)

mosquitto_pub : pub_client.o client_shared.o $(LIBRARIES)
	${CXX} $^ -o $@ ${LDFLAGS}

pub_client.o : pub_client.cpp
	${CXX} -c $^ -o $@ ${CFLAGS}

client_shared.o : client_shared.cpp
	${CXX} -c $^ -o $@ ${CFLAGS}

clean : 
	-rm -f *.o ${TARGETS}

#gcc -c -DVERSION="123" -I /home/stuartf/workspace/org.eclipse.mosquitto/lib pub_client.c
#gcc -c -DVERSION="123" -I /home/stuartf/workspace/org.eclipse.mosquitto/lib client_shared.c

#gcc -o mosquitto_pub pub_client.o client_shared.o -L/home/stuartf/workspace/org.eclipse.mosquitto/lib -l mosquitto
