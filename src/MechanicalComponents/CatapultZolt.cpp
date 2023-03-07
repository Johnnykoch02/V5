/**
 * @file CatapultZolt.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Catapult Shooter Mechanism that uses a Zolt-like reflex to shoot the Disc Game Pieces.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#include "../../include/MechanicalComponents/Shooters/Configurations/Catapult/CatapultZolt.hpp"
int CatapultZolt::Shoot(float delta) { 
    toggled = true;
    if (!this->shotComplete) {
        if (this->limitSwitch->get_new_press()) {
            this->shotComplete = true;
            this->pMotorX->move(0);
            this->pMotorY->move(0);
        }
        else {
            this->pMotorX->move(100);
            this->pMotorY->move(100);
        }
    }
    return 0;
}
int CatapultZolt::turnOn() {
    return 0;
}
int CatapultZolt::reset() {
    this->shotComplete = false;
    this->toggled = false;
    this->pMotorX->move(0);
    this->pMotorY->move(0);
    return 0;
}
bool CatapultZolt::shotCompleted() {
    return this->shotComplete;
}