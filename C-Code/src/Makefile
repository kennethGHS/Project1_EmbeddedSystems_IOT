CC = gcc
HSOURCE = ../include/
CSSOURCE = ../lib/
LIB = -lcurl -lpthread -lrt -ljson-c
SOURCES = test.c $(CSSOURCE)HTTPRequests.c $(CSSOURCE)ImageGet.c $(CSSOURCE)JsonAdmin.c $(CSSOURCE)ManualGPIO.c $(CSSOURCE)Monitoring.c
build_test1:
	gcc -I../include/ -I/usr/include/ -o Test $(SOURCES) $(LIB)