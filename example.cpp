#include <iostream>
#include <unistd.h>
using namespace std;

#include "Motor.h"
#include "driver/RoboteqDevice.h"
#include "config.h"

#define MOTOR_COUNT 5

int main() {

  RoboteqDevice device;

  device.Connect("/dev/ttyUSB0");
  usleep(10*1000);//10ms
  Motor* motors[MOTOR_COUNT];


  for(int i = 0; i < MOTOR_COUNT; i++) {
    motors[i] = new Motor(MOTOR_IDS[i], device, ENCODER_LOWER_LIMIT[i], ENCODER_HIGHER_LIMIT[i]);
  }
  // motors[0]->setPosition(500);
  motors[0]->goHome();
  // motors[1]->setPosition(1500);
  cout<<"Pos="<<motors[1]->readMotorPosition();
  // cout<<motors[0]->getId();
}
