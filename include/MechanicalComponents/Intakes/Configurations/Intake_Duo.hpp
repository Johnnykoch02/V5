/**
 * @file Intake_Duo.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Intake mechanism for Robots to take in Game Objects that utilizes two motors.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#ifndef INTAKE_DUO_H
#define INTAKE_DUO_H

#include "../intake.hpp"
#include "../../pros/apix.h" 


class Intake_Duo : public TerriBull::Intake {
    protected:
    pros::Motor* pMotorI,* pMotorJ;

    public:
    Intake_Duo(int i, bool iReverse, int j, bool jReverse, int _maxSpeed, int gearSet) : TerriBull::Intake(_maxSpeed, gearSet) {
        this->pType = "Intake-Duo";
        this->pMotorI = new pros::Motor(i, (pros::motor_gearset_e) this->gearSet, iReverse);
        this->pMotorJ = new pros::Motor(j, (pros::motor_gearset_e) this->gearSet, jReverse);
        this->pMotorI->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
    }

    int TurnOn(float dir); // button is held
    float getRPM() const;
    int TurnOff();
};     

#endif // INTAKE_H
