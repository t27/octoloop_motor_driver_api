#include <iostream>
#include <stdio.h>
#include <string.h>

#include "Motor.h"

using namespace std;

Motor::Motor(int id, RoboteqDevice &controller) {
  this->id = id;
  this->controller = controller;
}

Motor::Motor(int id, RoboteqDevice &controller, int lower_limit, int higher_limit) {
  this -> id = id;
  this -> controller = controller;
  this -> lower_limit = lower_limit;
  this -> higher_limit = higher_limit;
}


Motor::~Motor() {
  controller.Disconnect();
}

/**
 * Returns the current motors id
 * @return [Motor id]
 */
int Motor::getId() {
  return id;
}

/**
 * Returns the position value from the object variable(last known position)
 * @return [returns value of the position variable]
 */
int Motor::getPosition() {
  return current_position;
}

/**
 * Reads current position directly from the motor
 * @return [This returns the motor position as seen on the motor]
 */
int Motor::readMotorPosition() {
  int result = -1;
  int status;
  if((status = controller.GetValueId(id, _C, result)) == RQ_SUCCESS) {
    current_position = result;
    return result;
  } else {
    cout<<"Read Position Failure"<<endl;
    return -1;
  }

}

bool Motor::setPosition(int position) {
  int status;
  if (position >= lower_limit && position <= higher_limit) {
    if((status = controller.SetCommandId(id, _P, position)) == RQ_SUCCESS) {
      controller.SetCommandId(id, _P, position);
      return true;
    } else {
      cout<<"Set Position Failure - No response received - "<<status<<endl;
      return false;
    }
  } else {
    return false;
  }
}

bool Motor::goHome() {
  int status = controller.SetCommandId(id, _VAR, 1, 1);

  if((status) == RQ_SUCCESS) {
    return true;
  } else {
    cout<<"Go Home command Failure - "<<status<<endl;
    return false;
  }
}
