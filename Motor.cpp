#include <iostream>
#include <stdio.h>
#include <string.h>

#include "Motor.h"

using namespace std;

Motor::Motor(int id, RoboteqDevice &controller) {
  this->id_ = id;
  this->controller_ = controller;
}

/**
 * Parameterised contructor, lower_limit and higher_limit are used to limit values sent to the motor only
 */
Motor::Motor(int id, RoboteqDevice &controller, int lower_limit, int higher_limit, int max_speed) {
  this -> id_ = id;
  this -> controller_ = controller;
  this -> lower_limit_ = lower_limit;
  this -> higher_limit_ = higher_limit;
  this -> max_speed_ = max_speed;
}


Motor::~Motor() {
  controller_.Disconnect();
}

/**
 * Returns the current motor id
 * @return Motor id
 */
int Motor::getId() {
  return id_;
}


/**
 * Reads current position directly from the motor
 * @return Reads the  current position from the motor
 */
int Motor::getPosition() {
  int result = -1;
  int status;
  if((status = controller_.GetValueId(id_, _C, result)) == RQ_SUCCESS) {
    current_position_ = result;
    return result;
  } else {
#ifdef PRINTING_ON
    cout<<"[MotorDriverLib]Read Position Failure"<<endl;
#endif
    return -100;
  }
}


/**
 * Set the target position for the motor
 * @param  position Position(in Encoder Counts)
 * @return          Success/Failure
 */
bool Motor::setPosition(int position) {
  int status;
  if (position >= lower_limit_ && position <= higher_limit_) {
    if((status = controller_.SetCommandId(id_, _P, position)) == RQ_SUCCESS) {
      // controller_.SetCommandId(id_, _P, position);
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

/**
 * Set the next target position for the motor
 * @param  position Position(in Encoder Counts)
 * @return          Success/Failure
 */
bool Motor::setNextPosition(int position) {
  int status;
  if (position >= lower_limit_ && position <= higher_limit_) {
    if((status = controller_.SetCommandId(id_, _PX, position)) == RQ_SUCCESS) {
      // controller_.SetCommandId(id_, _P, position);
      return true;
    } else {
#ifdef PRINTING_ON
      cout<<"[MotorDriverLib]Set Next Position Failure - No response received - "<<status<<endl;
#endif
      return false;
    }
  } else {
    return false;
  }
}

/**
 * Read Motor Speed(RPM)
 * @return Motor Speed value in RPM
 */
int Motor::getSpeed() {
  int result = -1;
  int status;
  if((status = controller_.GetValueId(id_, _S, result)) == RQ_SUCCESS) {
    current_position_ = result;
    return result;
  } else {
#ifdef PRINTING_ON
    cout<<"[MotorDriverLib]Read Speed Failure"<<endl;
#endif
    return -1;
  }
}

/**
 * Set Motor Speed in RPM
 * @param  value Speed in RPM
 * @return       Success/Failure
 */
bool Motor::setSpeed(int value) {
  int status;
  if (abs(value) <= max_speed_) {
    if((status = controller_.SetCommandId(id_, _S, value)) == RQ_SUCCESS) {
      return true;
    } else {
#ifdef PRINTING_ON
      cout<<"[MotorDriverLib]Set Speed Failure - No response received - "<<status<<endl;
#endif
      return false;
    }
  } else {
    return false;
  }
}

/**
 * Set Motor Speed for the next trajectory in RPM
 * @param  value Speed in RPM
 * @return       Success/Failure
 */
bool Motor::setNextSpeed(int value) {
  int status;
  if (abs(value) <= max_speed_) {
    if((status = controller_.SetCommandId(id_, _SX, value)) == RQ_SUCCESS) {
      return true;
    } else {
#ifdef PRINTING_ON
      cout<<"[MotorDriverLib]Set Next Speed Failure - No response received - "<<status<<endl;
#endif
      return false;
    }
  } else {
    return false;
  }
}

bool Motor::destinationReached() {
  int result = -1;
  int status;
  if((status = controller_.GetValueId(id_, _DR, result)) == RQ_SUCCESS) {
    return result;
  } else {
#ifdef PRINTING_ON
    cout<<"[MotorDriverLib]Read DR Failure"<<endl;
#endif
    return -1;
  }
}

bool Motor::isLimitSwitchOn() {
  int result = -1;
  int status;
  if((status = controller_.GetValueId(id_, _DIN, 5, result)) == RQ_SUCCESS) {
    if(result == 1) {
      return false;
    } else {
      return true;
    }
  } else {
#ifdef PRINTING_ON
    cout<<"[MotorDriverLib]Read Lim Switch Failure"<<endl;
#endif
    return false;
  }
}

/**
 * Initiate Homing routine, this function will wait till homing is done
 * @return Success/Failure
 */
bool Motor::goHome() {
  bool origLimitSwitchState = isLimitSwitchOn();
  bool status = goHomeAsync();
  // int status = controller_.SetCommandId(id_, _VAR, 1, 1);
  int readValue = 1;
  while(readValue != 0) {
    controller_.GetValueId(id_, _VAR, 1, readValue);
    if (readValue == 0 && origLimitSwitchState) { // check limit switch if it was originally on
      cout<<"here1\n";
      bool limitSwitchNow = isLimitSwitchOn();
      // if it is still on, homing was successful, else send home command again
      if (!limitSwitchNow) {
        cout<<"here2\n";
        // cout<<"Home done and limit switch is still off\n";
        goHomeAsync();
        readValue = 1;
      }
    }
#ifdef PRINTING_ON
    cout<<"ReadValue="<<readValue<<"\n";
#endif
  }
  if(status) {
    return true;
  } else {
#ifdef PRINTING_ON
    cout<<"[MotorDriverLib]Go Home command Failure - "<<status<<endl;
#endif
    return false;
  }
}

/**
 * Initiate Homing routine asynchronously, returns immediately, call and forget
 * @return Success/Failure
 */
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
