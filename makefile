# DO NOT CONVERT TABS TO SPACES HERE

CC=g++

all: sample.o Motor.o

sample.o: RoboteqDevice.o ./driver/sample.cpp
	$(CC) RoboteqDevice.o ./driver/sample.cpp -o sample.o

RoboteqDevice.o: ./driver/RoboteqDevice.cpp
	$(CC) -c ./driver/RoboteqDevice.cpp

Motor.o: Motor.cpp
	$(CC) -c Motor.cpp

clean:
	rm *.o
