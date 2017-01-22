#include <iostream>
#include <stdio.h>
#include <string.h>

#include "Motor.h"

using namespace std;

Motor::Motor(int id, RoboteqDevice &controller) {
  this->id_ = id;
  this->controller_ = controller;
}

Motor::Motor(int id, RoboteqDevice &controller, int lower_limit, int higher_limit) {
  this -> id_ = id;
  this -> controller_ = controller;
  this -> lower_limit_ = lower_limit;
  this -> higher_limit_ = higher_limit;
}


Motor::~Motor() {
  controller_.Disconnect();
}

/**
 * Returns the current motors id
 * @return [Motor id]
 */
int Motor::getId() {
  return id_;
}

/**
 * Returns the position value from the object variable(last known position)
 * @return [returns value of the position variable]
 */
int Motor::getPosition() {
  return current_position_;
}

/**
 * Reads current position directly from the motor
 * @return [This returns the motor position as seen on the motor]
 */
int Motor::readMotorPosition() {
  int result = -1;
  int status;
  if((status = controller_.GetValueId(id_, _C, result)) == RQ_SUCCESS) {
    current_position_ = result;
    return result;
  } else {
#ifdef PRINTING_ON
    cout<<"[MotorDriverLib]Read Position Failure"<<endl;
#endif
    return -1;
  }
}

bool Motor::setPosition(int position) {
  int status;
  if (position >= lower_limit_ && position <= higher_limit_) {
    if((status = controller_.SetCommandId(id_, _P, position)) == RQ_SUCCESS) {
      controller_.SetCommandId(id_, _P, position);
      return true;
    } else {
#ifdef PRINTING_ON
      cout<<"[MotorDriverLib]Set Position Failure - No response received - "<<status<<endl;
#endif
      return false;
    }
  } else {
    return false;
  }
}

bool Motor::goHome() {
  int status = controller_.SetCommandId(id_, _VAR, 1, 1);
  int readValue = 1;
  while(readValue != 0) {
    controller_.GetValueId(id_, _VAR, 1, readValue);
#ifdef PRINTING_ON
    cout<<"ReadValue="<<readValue<<"\n";
#endif
  }
  if((status) == RQ_SUCCESS) {
    return true;
  } else {
#ifdef PRINTING_ON
    cout<<"[MotorDriverLib]Go Home command Failure - "<<status<<endl;
#endif
    return false;
  }
}

bool Motor::goHomeAsync() {
  int status = controller_.SetCommandId(id_, _VAR, 1, 1);
  if((status) == RQ_SUCCESS) {
    return true;
  } else {
#ifdef PRINTING_ON
    cout<<"[MotorDriverLib]Go Home command Failure - "<<status<<endl;
#endif
    return false;
  }
}
