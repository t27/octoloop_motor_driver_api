# DO NOT CONVERT TABS TO SPACES HERE

CC=g++

all: example.cpp sample.o Motor.o example.o

sample.o: RoboteqDevice.o ./driver/sample.cpp ./driver/RoboteqDevice.cpp
	$(CC) RoboteqDevice.o ./driver/sample.cpp -o sample.o

RoboteqDevice.o: ./driver/RoboteqDevice.cpp ./driver/RoboteqDevice.h
	$(CC) -c ./driver/RoboteqDevice.cpp

Motor.o: Motor.cpp Motor.h RoboteqDevice.o ./driver/RoboteqDevice.cpp
	$(CC) -c Motor.cpp

example.o: Motor.o RoboteqDevice.o ./driver/RoboteqDevice.cpp
	$(CC) Motor.o RoboteqDevice.o example.cpp -o example.o

clean:
	rm *.o
