#include
INCLUDE =-I ./

LIBS= -L/usr/local/lib/liblog4cplus.a -llog4cplus

#obj
OBJ:=$(patsubst %.cpp, %.o, $(wildcard *.cpp))
TARGET=../bin/server

CC = g++

%.o:%.c
	$(CC) -c $< -o $@ $(INCLUDE) 
$(TARGET):$(OBJ)
	$(CC) $< -o $@ $(LIBS) $(CFLAGS) 
all:$(TARGET)
	
clean:
	rm -rf *.o

