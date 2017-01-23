#include <iostream>
#include <unistd.h>
using namespace std;

#include "Motor.h"
#include "driver/RoboteqDevice.h"
#include "config.h"

#define MOTOR_COUNT 5

void testSpeed(Motor* motor) {

  int dr = 0;
  motor->setSpeed(5);
  motor->setPosition(2000);
  motor->setNextSpeed(5);
  motor->setNextPosition(5000);
  while(!dr) {
    dr = motor->destinationReached();
    if(dr == 1) {
      motor->setNextPosition(8000);
      dr = motor->destinationReached();
    }
    // dest reached flag is 1 when the motor has reached the first destination,
    // it resets after setNextPosition is called (check p134 roboteq manual"?DR")
    cout<<"destReached:"<<dr<<endl<<"speed:"<<motor->getSpeed();
  }
}
void basicMovement(Motor* motor) {
  // motors[0]->setPosition(500);
  motor->goHome();
  // motors[1]->setPosition(1500);
  cout<<"Pos="<<motor->readMotorPosition();
  // cout<<motors[0]->getId();
}


int main() {

  RoboteqDevice device;

  device.Connect("/dev/ttyUSB0");
  usleep(10*1000);//10ms
  Motor* motors[MOTOR_COUNT];


  for(int i = 0; i < MOTOR_COUNT; i++) {
    motors[i] = new Motor(MOTOR_IDS[i], device, ENCODER_LOWER_LIMIT[i], ENCODER_HIGHER_LIMIT[i], MOTOR_MAX_SPEED);
  }
  testSpeed(motors[0]);
}
