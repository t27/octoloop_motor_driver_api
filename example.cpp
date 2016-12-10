#include <iostream>

using namespace std;

#include "Motor.h"
#include "driver/RoboteqDevice.h"

#define MOTOR_COUNT 5

int main() {

  RoboteqDevice device;

  device.Connect("/dev/ttyUSB0");

  Motor* motors[MOTOR_COUNT];

  for(int i = 0; i < MOTOR_COUNT; i++) {
    motors[i] = new Motor(i, device, 0, 16383);
  }
  motors[0]->setPosition(1000);
  cout<<motors[0]->getId();
}
