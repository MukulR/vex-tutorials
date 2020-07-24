#include "main.h"
/**
 * These are some handy movement functions for auton that I used last year.
 * The turning functions require an inertial sensor on your robot.
 * Motor Constructor: pros::Motor(<portNumber>, <cartridgeGearset>, <reversedOrNot>, <encoderUnits>)
 * cartridge gearset corresponds with the cartridge in your motor. Default (green) is _18
 * The functions below are designed to work with encoder units of degrees
 * If you need some assistance with the functions below, please contact me.
*/

#define IMU_PORT 1 //feel free to change
#define TURN_SPEED 40 //feel free to change, or parameterize turn speed in both turn functions

// Inertial sensor initialization
// The inertial sensor is essentially a glorified gyroscope that can track your robot's movment in all directions.
// For this case, we will use get_heading() which gives us the proper readings for turns
pros::Imu imu(IMU_PORT);


// Feel free to change these to match your robot. If you have questions let me know on discord/email
// In the long run I would recommened making a motor initialization class. Refer to my github:
// https://github.com/MukulR/Tower-Takeover/tree/master/sigevent to see how I did that, specifically the
// motordefs.cpp and motordefs.hpp files. It makes the code a lot more organized and easy to use everywhere.
pros::Motor left_mtr_f (1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_mtr_b (2, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_mtr_f (3, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_mtr_b (4, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);





void turnRobotLeft(double degrees){
	left_mtr_f.move(-TURN_SPEED);
	left_mtr_b.move(-TURN_SPEED);
	right_mtr_f.move(TURN_SPEED);
	right_mtr_b.move(TURN_SPEED);
	while (imu.get_heading() - degrees < 0); // correction loop (ask on discord if you need help understanding why/how this works)
	while (imu.get_heading() > 360 - degrees); // turn the robot the accurate amount of degs
	// hard break below (turn in the opposite direction for 50 milliseconds to prevent coasting)
	left_mtr_f.move(TURN_SPEED);
	left_mtr_b.move(TURN_SPEED);
	right_mtr_f.move(-TURN_SPEED);
	right_mtr_b.move(-TURN_SPEED);
	pros::Task::delay(50);
	left_mtr_f.move(0);
	left_mtr_b.move(0);
	right_mtr_f.move(0);
	right_mtr_b.move(0);
}

void turnRobotRight(double degrees){
	left_mtr_f.move(TURN_SPEED);
	left_mtr_b.move(TURN_SPEED);
	right_mtr_f.move(-TURN_SPEED);
	right_mtr_b.move(-TURN_SPEED);
	while (degrees - imu.get_heading() < 0); // bring the robot to 0 heading (ask on discord if you need help understanding why/how this works)
	while (imu.get_heading() < degrees); // turn the robot the accurate amount of degs
	// hard break below (turn in the opposite direction for 50 milliseconds to prevent coasting)
	left_mtr_f.move(-TURN_SPEED);
	left_mtr_b.move(-TURN_SPEED);
	right_mtr_f.move(TURN_SPEED);
	right_mtr_b.move(TURN_SPEED);
	pros::Task::delay(50);
	left_mtr_f.move(0);
	left_mtr_b.move(0);
	right_mtr_f.move(0);
	right_mtr_b.move(0);
}

// This driving function takes the distance to travel in degrees, and a speed to travel at
// Then, the encoders on the front wheels (wheels I'm using to determine the distance we travel)
// are reset. Next, we assign power to the drive motors.
// We use a while(true) to continuously check to see if the distance we've traveled has reached the desired amount.
// The distanced traveled is calculated by averaging the two front motors' individual distance traveled.
// Then, a hard break is applied by moving the robot in reverse for 50 milliseconds to prevent coasting,
// before finally breaking the loop and function.
// Make sure the degrees and speed input are positive.

void driveRobotForward(int degrees, int speed){
	left_mtr_f.tare_position();
	right_mtr_f.tare_position();
	left_mtr_f.move(speed);
	left_mtr_b.move(speed);
	right_mtr_f.move(speed);
	right_mtr_b.move(speed);
	while(true){
		int degreesTraveled = (left_mtr_f.get_position() + right_mtr_f.get_position()) / 2;
		if (degreesTraveled >= degrees){
			left_mtr_f.move(-speed);
			left_mtr_b.move(-speed);
			right_mtr_f.move(-speed);
			right_mtr_b.move(-speed);
			pros::Task::delay(50);
			left_mtr_f.move(0);
			left_mtr_b.move(0);
			right_mtr_f.move(0);
			right_mtr_b.move(0);
			break;
		}
	}
}

//Similarly, for driving backwards. Make sure the speed input and degrees input are negative.

void driveRobotBack(int degrees, int speed){
	left_mtr_f.tare_position();
	right_mtr_f.tare_position();
	left_mtr_f.move(speed);
	left_mtr_b.move(speed);
	right_mtr_f.move(speed);
	right_mtr_b.move(speed);
	while(true){
		int degreesTraveled = (left_mtr_f.get_position() + right_mtr_f.get_position()) / 2;
		if (abs(degreesTraveled) >= abs(degrees)){
			left_mtr_f.move(-speed);
			left_mtr_b.move(-speed);
			right_mtr_f.move(-speed);
			right_mtr_b.move(-speed);
			pros::Task::delay(50);
			left_mtr_f.move(0);
			left_mtr_b.move(0);
			right_mtr_f.move(0);
			right_mtr_b.move(0);
			break;
		}
	}
}
