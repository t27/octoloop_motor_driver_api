# DO NOT CONVERT TABS TO SPACES HERE

CC=g++

all: sample.o

sample.o: RoboteqDevice.o sample.cpp
	$(CC) RoboteqDevice.o sample.cpp -o sample.o

RoboteqDevice.o: RoboteqDevice.cpp
	$(CC) -c RoboteqDevice.cpp

clean:
	rm *.o
