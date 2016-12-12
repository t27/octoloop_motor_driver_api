#include <iostream>
#include <stdio.h>
#include <string.h>

#include "./driver/RoboteqDevice.h"
#include "./driver/ErrorCodes.h"
#include "./driver/Constants.h"

using namespace std;

class Motor {

private:
  int id;
  int current_position;
  int target_position;
  RoboteqDevice controller;
  int lower_limit;
  int higher_limit;

public:
  bool setPosition(int position);
  int getPosition();
  int readMotorPosition();
  int getId();
  bool goHome();

  /*
    Expects a controller obj which is initialised prevously.
   */
  Motor(int id, RoboteqDevice controller);
  Motor(int id, RoboteqDevice controller, int lower_limit, int higher_limit);

  ~Motor();
};
