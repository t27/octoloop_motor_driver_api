# DO NOT CONVERT TABS TO SPACES HERE

CC=g++

all: homing.cpp example.cpp sample.o Motor.o example.o homing.o

sample.o: RoboteqDevice.o ./driver/sample.cpp ./driver/RoboteqDevice.cpp
	$(CC) RoboteqDevice.o ./driver/sample.cpp -o sample.o

RoboteqDevice.o: ./driver/RoboteqDevice.cpp ./driver/RoboteqDevice.h
	$(CC) -c ./driver/RoboteqDevice.cpp

Motor.o: Motor.cpp Motor.h RoboteqDevice.o ./driver/RoboteqDevice.cpp
	$(CC) -c Motor.cpp

example.o: Motor.o RoboteqDevice.o ./driver/RoboteqDevice.cpp
	$(CC) Motor.o RoboteqDevice.o example.cpp -o example.o

homing.o: Motor.o RoboteqDevice.o ./driver/RoboteqDevice.cpp
		$(CC) Motor.o RoboteqDevice.o homing.cpp -o homing.o

clean:
	rm *.o
