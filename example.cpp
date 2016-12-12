#include <iostream>

using namespace std;

#include "Motor.h"
#include "driver/RoboteqDevice.h"
#include "config.h"

#define MOTOR_COUNT 5

int main() {

  RoboteqDevice device;

  device.Connect("/dev/ttyUSB0");

  Motor* motors[MOTOR_COUNT];

  for(int i = 0; i < MOTOR_COUNT; i++) {
    motors[i] = new Motor(MOTOR_IDS[i], device, ENCODER_LOWER_LIMIT[i], ENCODER_HIGHER_LIMIT[i]);
  }
  motors[0]->setPosition(500);
  cout<<motors[0]->getId();
}

// ToDo make a plain text config file and load it before initialising motors. Config file will have Ids and encoder limits for the motors. Maybe more params later.
