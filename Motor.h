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

public:
  int setPosition();
  int getPosition();
  int getId();

  Motor(int id, RoboteqDevice controller);
  ~Motor();
};
