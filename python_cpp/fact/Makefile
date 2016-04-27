all:
	gcc -fpic -c -I/usr/include/python2.7 example.c wrapper.c
	gcc -shared example.o wrapper.o -o examplemodule.so

