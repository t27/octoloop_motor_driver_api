# DO NOT CONVERT TABS TO SPACES HERE

CC=g++

all: sample.o Motor.o example.o

sample.o: RoboteqDevice.o ./driver/sample.cpp
	$(CC) RoboteqDevice.o ./driver/sample.cpp -o sample.o

RoboteqDevice.o: ./driver/RoboteqDevice.cpp
	$(CC) -c ./driver/RoboteqDevice.cpp

Motor.o: Motor.cpp RoboteqDevice.o
	$(CC) -c Motor.cpp

example.o: Motor.o RoboteqDevice.o
	$(CC) Motor.o RoboteqDevice.o example.cpp -o example.o

clean:
	rm *.o
