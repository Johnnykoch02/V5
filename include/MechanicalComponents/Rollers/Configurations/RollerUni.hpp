/**
 * @file Intake_Uni.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Intake mechanism for Robots to take in Game Objects that utilizes a single motor.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#ifndef INTAKE_UNI_H
#define INTAKE_UNI_H

#include "../roller.hpp"
#include "../../pros/apix.h" 


class Intake_Uni : public TerriBull::Intake {
    protected:
    pros::Motor* pMotorI;

    public:
    Intake_Uni(int i, int _maxSpeed) : TerriBull::Intake(_maxSpeed) {
        this->pMotorI = new pros::Motor(i);
    }

    int TurnOn(int dir); // button is held
    int TurnOff();
};     

#endif // INTAKE_H
