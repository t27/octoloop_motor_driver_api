#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>

#include "./driver/RoboteqDevice.h"
#include "./driver/ErrorCodes.h"
#include "./driver/Constants.h"

using namespace std;

class Motor {

private:
  int id_;
  int current_position_;
  int target_position_;
  RoboteqDevice controller_;
  int lower_limit_;
  int higher_limit_;
  int max_speed_;

public:
  int getId();

  bool setPosition(int position);
  int getPosition();

  bool setNextPosition(int position);

  int getSpeed();
  bool setSpeed(int value);
  bool setNextSpeed(int value);


  bool destinationReached();

  bool isLimitSwitchOn();
  bool goHome();
  bool goHomeAsync();

  /*
    Expects a controller obj which is initialised prevously.
   */
  Motor(int id, RoboteqDevice &controller);
  Motor(int id, RoboteqDevice &controller, int lower_limit, int higher_limit, int max_speed);

  ~Motor();
};
