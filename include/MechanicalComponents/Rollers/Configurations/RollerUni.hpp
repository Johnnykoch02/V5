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
#ifndef ROLLER_UNI_H
#define ROLLER_UNI_H

#include "../roller.hpp"
#include "../../pros/apix.h" 


class Roller_Uni : public TerriBull::Roller {
    protected:
    pros::Motor* pMotorI;

    public:
    Roller_Uni(int i, bool iReverse, int _maxSpeed), : TerriBull::Roller(_maxSpeed) {
        this->pMotorI = new pros::Motor(i, iReverse);
    }

    int Spin(int direction, float time);
    int SpinToPos(float pos);
    void reset();
};     

#endif // INTAKE_H
