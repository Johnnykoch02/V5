/**
 * @file TerriBull.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief TerriBull Single Robotics Library implementation designed to run on
 * the University of South Florida VEX BullBots seamlessly across all devices.
 *
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "../../include/TerriBull/TerriBull.hpp"

#ifndef __TERRIBULL_CONSTANTS__
#define __TERRIBULL_CONSTANTS__
namespace TerriBull {
  int const MAX_VOLTAGE = 127;
  float const PI =  3.14159;
  int const RED_REVS = 300;
  int const GREEN_REVS = 900;
  int const BLUE_REVS = 1800;
  std::map<std::string, int> GEAR_ENCODER = {
    {"RED", pros::E_MOTOR_GEARSET_36},
    {"GREEN", pros::E_MOTOR_GEARSET_18},
    {"BLUE", pros::E_MOTOR_GEARSET_06},
    {"SpecialFlyWheel", 3},
  };
  std::map<int, int> ENCODER_UNIT {
    {pros::E_MOTOR_GEARSET_06, RED_REVS},
    {pros::E_MOTOR_GEARSET_18, GREEN_REVS},
    {pros::E_MOTOR_GEARSET_36, BLUE_REVS},
    {3, 10800},/*Speical */
  };
};
#endif
#ifndef __TERRIBULL_GLOBALS__
#define __TERRIBULL_GLOBALS__
namespace TerriBull {
Logger logger("/usd/logfile.log"); /* Global Logger */
::pros::Controller controller(::pros::E_CONTROLLER_MASTER); /* Global Controller */
pros::Imu mu(11);

pros::lcd::

};
#endif
#ifndef __TERRIBULL_FUNCTIONS__
#define __TERRIBULL_FUNCTIONS__
namespace TerriBull {
    float DEG2RAD( const float deg )
    {
    	return deg * PI/180;
    }
    float RAD2DEG( const float rad )
    {
    	return (float)fmod(rad * 180/PI, 360);
    }

    float GetDTheta(float tf, float ti) {
    float positiveDTheta = fmod((tf+360)-ti, 360.0);
    float negativeDTheta = -360 + positiveDTheta;

    if (fabs(positiveDTheta) <= fabs(negativeDTheta))
      return positiveDTheta;
    else return negativeDTheta;

    }
};
#endif