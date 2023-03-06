/**
 * @file intake.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Intake mechanism for Robots to take in Game Objects.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#ifndef INTAKE_H
#define INTAKE_H

#include "../../TerriBull/TerriBull.hpp"

class TerriBull::Intake : public TerriBull::MechanicalComponent {
    protected:
    bool toggled;
    int currentDir;
    int maxSpeed;
    public:
    Intake(int _maxSpeed, int gearSet) : TerriBull::MechanicalComponent(gearSet), maxSpeed(_maxSpeed), currentDir(0) {}

    virtual int TurnOn(int dir) = 0; // button is held
    virtual int TurnOff() = 0;

};     

#endif // INTAKE_H
