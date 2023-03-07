/**
 * @file main.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Main Target ran on V5
 *
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */


// namespace BULL2_XD {
	// pros::Motor pMotorA (1, pros::E_MOTOR_GEARSET_18, false);
	// pros::Motor pMotorB (2, pros::E_MOTOR_GEARSET_18, false);
	// pros::Motor pMotorC (3, pros::E_MOTOR_GEARSET_18, true);
	// pros::Motor pMotorD (4, pros::E_MOTOR_GEARSET_18, true);
// };

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
	} else {
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
TerriBull::RoboController controlSys;



void initialize() {
	pros::lcd::initialize();
	controlSys.Init();
}


void autonomous() {
	while (true) {
		controlSys.Run();
    }
}



void opcontrol() {
	while (true) {
		controlSys.Run();
    }
}
