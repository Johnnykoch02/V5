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
    Intake_Duo(int i, int j, int _maxSpeed) : TerriBull::Intake(_maxSpeed) {
        this->pMotorI = new pros::Motor(i);
        this->pMotorJ = new pros::Motor(j);
    }

    int TurnOn(int dir); // button is held
    int TurnOff();
};     

#endif // INTAKE_H
