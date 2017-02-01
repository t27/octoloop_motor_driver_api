#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

#include "Motor.h"
#include "driver/RoboteqDevice.h"
#include "config.h"


int main() {
  cout<<"-------------------------\n";
  cout<<"---Octoloop Homing CLI---\n";
  cout<<"-------------------------\n";
  bool exit = false;
  int motor_id = -1;
  while(!(motor_id > 0 && motor_id < 6)) {
    cout<<"Enter motor id(1,2,3,4 or 5)\n";
    cin>>motor_id;
  }
  RoboteqDevice device;

  device.Connect("/dev/ttyUSB0");
  usleep(10*1000);//10ms
  Motor* motors[5];

  for(int i = 0; i < 5; i++) {
    motors[i] = new Motor(MOTOR_IDS[i], device, ENCODER_LOWER_LIMIT[i], ENCODER_HIGHER_LIMIT[i], MOTOR_MAX_SPEED);
  }
  do {
    int choice;
    int inp;
    cout<<"\n\nMenu\n"<<"1.Go Home\n"<<"2.Change Current Motor Id\n"<<"3.Exit\n";
    cin>>choice;
    switch(choice) {
      case 1: motors[motor_id-1]->goHome(); break;
      case 2:
        cout<<"Enter new id..";
        cin>>inp;
        if(inp < 6 && inp > 0) {
          motor_id = inp;
        } else {
          cout<<"Id must be 1,2,3,4 or 5\n";
        }
        break;
      case 3: exit = true; break;
      default: cout<<"Invalid input\n";
    }
  } while(!exit);
  device.Disconnect();
}
