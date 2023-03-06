/**
 * @file ProsAPI.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief API for pros functions to be called throughout the program and from external modules
 *     
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../include/TerriBull/ProsAPI.h"
/* 
 * X_Drive Configuration for BULL2
 */
// pros::Motor m1(1, pros::E_MOTOR_GEARSET_18, false);
// pros::Motor m2(2, pros::E_MOTOR_GEARSET_18, false);
// pros::Motor m3(3, pros::E_MOTOR_GEARSET_18, true);
// pros::Motor m4(4, pros::E_MOTOR_GEARSET_18, true);

pros::Imu mu(11);
mu.tare();
/*
 *
*/

// void x_drive_motors_set_voltages(int v1, int v2, int v3, int v4) {
//     m1.move(v1);
//     m2.move(v2);
//     m3.move(v3);
//     m4.move(v4);
// }

// pros::Motor m5(1, pros::E_MOTOR_GEARSET_18, false);
// pros::Motor m6(3, pros::E_MOTOR_GEARSET_18, true);
// pros::Motor m7(2, pros::E_MOTOR_GEARSET_18, false);
// pros::Motor m8(4, pros::E_MOTOR_GEARSET_18, false);
// pros::Motor m9(6, pros::E_MOTOR_GEARSET_18, true);
// pros::Motor m10(5, pros::E_MOTOR_GEARSET_18, false);