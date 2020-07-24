#include "main.h"

// This example program assumes that there are no reveresed motors.
// Please change the motor definitions accordingly if there are.
// This can be done by changed the third parameter to True, if you would
// like it to be reversed.

// In the long run I would recommened making a motor initialization class. Refer to my github:
// https://github.com/MukulR/Tower-Takeover/tree/master/sigevent to see how I did that, specifically the
// motordefs.cpp and motordefs.hpp files. It makes the code a lot more organized and easy to use everywhere.
pros::Motor left_mtr_f (1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_mtr_b (2, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_mtr_f (3, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_mtr_b (4, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor intake_1 (5, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor intake_2 (6, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor tower_1 (7, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor tower_2 (8, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);


pros::Controller master(pros::E_CONTROLLER_MASTER);

bool braked = false;

void drive(void* param){
  while(true) {
    int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    float scaledTurn = ((turn * 100) * 0.75) / 100;
    float leftMtrVals = (forward + scaledTurn);
    float rightMtrVals = -(scaledTurn - forward);
    if(leftMtrVals > 127){
      leftMtrVals = 127;
    }
    if(leftMtrVals < -127){
      leftMtrVals = -127;
    }
    if(rightMtrVals > 127){
      rightMtrVals = 127;
    }
    if(rightMtrVals < -127){
      rightMtrVals = -127;
    }
    float midMtrVals = floor((leftMtrVals + rightMtrVals) / 2);
    if(!braked){
      left_mtr_f.move(leftMtrVals);
      left_mtr_b.move(leftMtrVals);
      right_mtr_f.move(rightMtrVals);
      right_mtr_b.move(rightMtrVals);
    }
    // The below delay is required for tasks to work in PROS.
    pros::Task::delay(10);
  }
}


// Press button L2 (bottom left) to start intake, and press again to stop.
// Press and hold y to outtake

void intake(void* param){
  static bool intakeStarted = false;
  while(true){
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
      if(!intakeStarted){
        intake_1.move(127);
        intake_2.move(127);
        intakeStarted = true;
      } else if(intakeStarted){
        intake_1.move(0);
        intake_2.move(0);
        intakeStarted = false;
      }
      pros::Task::delay(300);
    }
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
      while(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
        intake_1.move(-127);
        intake_2.move(-127);
      }
      intake_1.move(0);
      intake_2.move(0);
    }
    pros::Task::delay(10);
  }
}

// Press R2 (bottom right) to start tower, press again to stop
// Press and hold X to slowly outtake

void tower(void* param){
  static bool towerStarted = false;
  while(true){
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
      if(!towerStarted){
        tower_1.move(127);
        tower_2.move(127);
        towerStarted = true;
      } else if(towerStarted){
        tower_1.move(0);
        tower_2.move(0);
        towerStarted = false;
      }
      pros::Task::delay(300);
    }
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
      while(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
        tower_1.move(-127);
        tower_2.move(-127);
      }
      tower_1.move(0);
      tower_2.move(0);
    }
    pros::Task::delay(10);
  }
}

void opcontrol() {
	pros::Task intakeTask(intake);
	pros::Task towerTask(tower);
  pros::Task driveTask(drive);
}
