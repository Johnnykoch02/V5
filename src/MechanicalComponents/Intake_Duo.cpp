/**
 * @file Intake_Uni.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Intake mechanism for Robots to take in Game Objects that utilizes a single motor.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#include "../../include/MechanicalComponents/Intakes/Configurations/Intake_Duo.hpp"


int Intake_Duo::TurnOn(int dir) {
    this->pMotorI->move(this->maxSpeed*dir);
    this->pMotorJ->move(this->maxSpeed*dir);
    this->currentDir = dir;
    this->toggled = true;
    return 0;
}
int Intake_Duo::TurnOff() {
    this->pMotorI->move(0);
    this->pMotorJ->move(0);
    this->currentDir = 0;
    this->toggled = false;
    return 0;
}