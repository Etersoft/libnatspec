datadir=/usr/share
CC = gcc
CXX = g++
CFLAGS = -g -Wall -O0 -DHAVE_MMAP
CXXFLAGS = $(CFLAGS) -ansi \
           `pkg-config --cflags glib-2.0` \
	   -DSTARDICT_DATA_DIR=\""$(datadir)/stardict"\" -DHAVE_NATSPEC
LDFLAGS = `pkg-config --libs glib-2.0 libnatspec` -lz

.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@  $<

.c.o:
	$(CC) -c $(CFLAGS) -o $@  $<

sdcv: main.o libcore.o lib.o utils.o distance.o dictziplib.o
	$(CXX) -o$@ $^ $(LDFLAGS)	

lib.o: lib.cpp lib.h

libcore.o: libcore.cpp libcore.hpp

.PHONY: clean

clean:
	rm -f *.o *~ sdcv
