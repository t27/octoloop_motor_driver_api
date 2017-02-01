#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

#include "Motor.h"
#include "driver/RoboteqDevice.h"
#include "config.h"

#define MOTOR_COUNT 5
#define JOINT_COUNT 5
struct TrajectorySegment
{
 double positions[5];
 double velocities[5];
};
///////////////////////////////////////////////////////////////////////////////
/// Joint Trajectory representation
///////////////////////////////////////////////////////////////////////////////
struct JointTrajectory {
   std::vector< TrajectorySegment > points;
};

int radPerSecToRPM(double radpersec) {
  double rpm = (radpersec/6.28)*60;
  return (int)rpm;
}

int radToEncoderCounts(double angleRads) {
  // map angle values from -3.14 - 3.14 to 0 to 16383
  // Mapping ranges - From Arduino's map implementation
  // ans = (angleRads - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  double in_min = -3.1415, in_max = 3.1415, out_min = 0.0, out_max = 16383.0;
  double value = (angleRads - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return (int)value;
}

double getAngleValue(double encoderValue) {
  double out_min = -3.1415, out_max = 3.1415, in_min = 0.0, in_max = 16383.0;
  double value = (encoderValue - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return value;
}

int getNthBit(int num, int bit) {
  return ((num >> bit) & 1);
}

int setNthBit(int num, int bit, int val) { // LSB is 0
  val = val>=1 ? 1 : 0;
  if (val >= 1) {
    num |= 1 << bit;
  } else {
    num &= ~(1 << bit);
  }
  return num;
}


int getDestReached(Motor** motors) {
  int dest = 0;
  for(int i = 0; i < JOINT_COUNT; i++) {
    int DR = motors[i]->destinationReached();
    cout<<"Motor"<<i<<" DR="<<DR<<endl;
    dest = setNthBit(dest, i, DR);
  }
  return dest;
}

int getDestReachedPosition(Motor** motors, TrajectorySegment dest) {
  int result=0;
  for(int i = 0; i < 1; i++) {
    int position = motors[i]->getPosition();
    int DR=0;
    cout<<"TArget Pos - Actual Pos =="<<radToEncoderCounts(dest.positions[i])<<" - "<<position<<"="<<abs(radToEncoderCounts(dest.positions[i])-position)<<endl;
    if (abs(radToEncoderCounts(dest.positions[i])-position) < 10) {
      DR=1;
    }
    cout<<"Motor"<<i<<" DR="<<DR<<endl;
    result = setNthBit(result, i, DR);
  }
  return result;
}

void executeTrajectoryNoDR(JointTrajectory traj, Motor** motors_) {
  int joint_number = 0,waypoint = 0;
  int dest_reached = 0;// last 5 bits of DR represent the dest reached flag for each motor;, i.e. 0 = none reached, 31 = all reached

  for(joint_number = 0; joint_number < 5; joint_number++) { // Joints Array
    // cout<<"jointNum"<<joint_number<<endl;
    // Set the current speed and position
    cout<<"Base Velocity"<<(radPerSecToRPM(traj.points[waypoint].velocities[joint_number]))<<endl;
    cout<<"Base position"<<(radToEncoderCounts(traj.points[waypoint].positions[joint_number]))<<endl;
    motors_[joint_number]->setSpeed(radPerSecToRPM(traj.points[waypoint].velocities[joint_number]));
    motors_[joint_number]->setPosition(radToEncoderCounts(traj.points[waypoint].positions[joint_number]));
  }
  waypoint++;
  while(waypoint < 5) {
      // dest_reached = getDestReached(motors_);// Load the flags
      dest_reached = getDestReachedPosition(motors_, traj.points[waypoint-1]);

      if (dest_reached % 2 == 1) {// 0b11111 //i.e. All motors have reached the destination
        // Now load the next positions
        cout<<"Reached destination"<<waypoint<<"\n";

        for(joint_number = 0; joint_number < 5; joint_number++) { // Joints Array
          // Set the next speed and position
          motors_[joint_number]->setSpeed(radPerSecToRPM(traj.points[waypoint].velocities[joint_number]));
          motors_[joint_number]->setPosition(radToEncoderCounts(traj.points[waypoint].positions[joint_number]));
          // cout<<"Next Velocity -"<<waypoint<<"="<<(radPerSecToRPM(traj.points[waypoint+1].velocities[joint_number]))<<endl;
          cout<<"Next position -"<<waypoint<<"="<<(radToEncoderCounts(traj.points[waypoint].positions[joint_number]))<<endl;
        }
        usleep(50000);//50ms
        waypoint++;
        dest_reached=0;
      } else {
        // cout<<"Waiting for all motors to reach destination\n";
      }
  }
}



void executeTrajectory(JointTrajectory traj, Motor** motors_) {
  int joint_number = 0,waypoint = 0;
  int dest_reached = 0;// last 5 bits of DR represent the dest reached flag for each motor;, i.e. 0 = none reached, 31 = all reached

  while(waypoint < 4) { // Traverse through trajectory points
      // cout<<"waypoint"<<waypoint<<endl<<"----------------"<<endl;
      if (waypoint == 0) {

        for(joint_number = 0; joint_number < 5; joint_number++) { // Joints Array
          // cout<<"jointNum"<<joint_number<<endl;
          // Set the current speed and position
          // cout<<"Base Velocity"<<(radPerSecToRPM(traj.points[waypoint].velocities[joint_number]))<<endl;
          cout<<"Base position"<<(radToEncoderCounts(traj.points[waypoint].positions[joint_number]))<<endl;
          // motors_[joint_number]->setSpeed(radPerSecToRPM(traj.points[waypoint].velocities[joint_number]));
          motors_[joint_number]->setPosition(radToEncoderCounts(traj.points[waypoint].positions[joint_number]));

          // Set the next speed and position
          // motors_[joint_number]->setNextSpeed(radPerSecToRPM(traj.points[waypoint + 1].velocities[joint_number]));
          motors_[joint_number]->setNextPosition(radToEncoderCounts(traj.points[waypoint + 1].positions[joint_number]));
          // cout<<"Next Velocity -"<<waypoint<<"="<<(radPerSecToRPM(traj.points[waypoint+1].velocities[joint_number]))<<endl;
          cout<<"Next position -"<<waypoint<<"="<<(radToEncoderCounts(traj.points[waypoint+1].positions[joint_number]))<<endl;
        }
        waypoint++;
      }
      // dest_reached = getDestReached(motors_);// Load the flags
      dest_reached = getDestReachedPosition(motors_, traj.points[waypoint]);

      if (dest_reached % 2 == 1) {// 0b11111 //i.e. All motors have reached the destination
        // Now load the next positions
        cout<<"Reached destination"<<waypoint<<"\n";

        for(joint_number = 0; joint_number < 5; joint_number++) { // Joints Array
          // Set the next speed and position
          // motors_[joint_number]->setNextSpeed(radPerSecToRPM(traj.points[waypoint + 1].velocities[joint_number]));
          motors_[joint_number]->setNextPosition(radToEncoderCounts(traj.points[waypoint + 1].positions[joint_number]));
          // cout<<"Next Velocity -"<<waypoint<<"="<<(radPerSecToRPM(traj.points[waypoint+1].velocities[joint_number]))<<endl;
          cout<<"Next position -"<<waypoint<<"="<<(radToEncoderCounts(traj.points[waypoint+1].positions[joint_number]))<<endl;
        }
        usleep(50000);//50ms
        waypoint++;
        dest_reached=0;
      } else {
        // cout<<"Waiting for all motors to reach destination\n";
      }
  }
}

JointTrajectory createSimpleTrajectory() {
  JointTrajectory traj;
  TrajectorySegment newSeg;

  for(int i = 0;i<5;i++) {
    newSeg.positions[i] = -3.14;
    newSeg.velocities[i] = 0.4;
  }
  for(int i=0;i<5;i++) {

    if(i%2==0) {
      newSeg.velocities[0]+=1;
      newSeg.positions[0]+=0.7;
    } else {
      newSeg.velocities[0]-=1;
      newSeg.positions[0]-=0.3;
    }
    newSeg.positions[1]+= 0.4;
    newSeg.velocities[1]+= 0.2;
    traj.points.push_back(newSeg);
  }
  return traj;
  // executeTrajectory(traj);
}

//------------------------------------
void testSpeed(Motor* motor) {

  int dr = 0;
  motor->setSpeed(5);
  motor->setPosition(2000);
  motor->setNextSpeed(10);
  motor->setNextPosition(5000);
  while( 1 || !dr) {
    dr = motor->destinationReached();
    if(dr == 1) {
      cout<<"Setting Next position 2nd time";
      motor->setNextSpeed(5);
      motor->setNextPosition(8000);
      // dr=0;
      dr = motor->destinationReached();
    }
    // dest reached flag is 1 when the motor has reached the first destination,
    // it resets after setNextPosition is called (check p134 roboteq manual"?DR")
    cout<<"destReached:"<<dr<<endl<<"speed:"<<motor->getSpeed();
  }
}
//------------------------------------
void basicMovement(Motor* motor) {
  // motors[0]->setPosition(500);
  motor->goHome();
  // motors[1]->setPosition(1500);
  cout<<"Pos="<<motor->getPosition();
  // cout<<motors[0]->getId();
}

void singleMotorTrajectory(Motor* motor) {
  int traj[] = {1000, 3000, 5000, 6000, 8000, 9000, 10000};

// Without using next position function
  // motor->setPosition(traj[0]);
  // int index=1;
  // while(index<7) {
  //   int current_pos = motor->getPosition();
  //   cout<<"Current = "<<current_pos<<"  Target="<<traj[index-1]<<"Diff"<<abs(current_pos-traj[index-1])<<endl;
  //   if(abs(current_pos-traj[index-1])<100) {
  //     cout<<"Reached "<<index-1<<"th Point in Trajectory, loading"<<traj[index]<<endl;
  //     motor->setPosition(traj[index]);
  //     index++;
  //   }
  // }

//using nect position function // doesnt work, must have DR flag set
  motor->setPosition(traj[0]);
  motor->setNextPosition(traj[1]);
  int index=2;
  int dr=0;
  while(index<7) {
    dr = motor->destinationReached();
    if(dr == 1) {
      cout<<"Reached "<<index-2<<"th Point in Trajectory, loading"<<traj[index]<<endl;
      motor->setNextPosition(traj[index]);
      index++;
    }
  }

}

int main() {

  RoboteqDevice device;

  device.Connect("/dev/ttyUSB0");
  usleep(10*1000);//10ms
  Motor* motors[MOTOR_COUNT];


  for(int i = 0; i < MOTOR_COUNT; i++) {
    motors[i] = new Motor(MOTOR_IDS[i], device, ENCODER_LOWER_LIMIT[i], ENCODER_HIGHER_LIMIT[i], MOTOR_MAX_SPEED);
  }
  // testSpeed(motors[0]);
  // executeTrajectory(createSimpleTrajectory(), motors);
  // singleMotorTrajectory(motors[1]);
  motors[1]->goHome();
  cout<<"Home done";
}
