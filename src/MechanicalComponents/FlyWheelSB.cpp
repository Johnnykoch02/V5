/**
 * @file FlyWheelSB.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Uses a Flywheel to propell the Disc Game Pieces into the goal.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#include "../../include/MechanicalComponents/Shooters/Configurations/FlyWheel/FlyWheelSB.hpp"
int FlyWheelSB::Shoot(float delta) { 
    toggled = true;
    this->sumTime+=delta;
    this->pMag->update(delta);
    return 0;
}

int FlyWheelSB::Load(float delta, void* args) {
    toggled = true;
    this->sumTime+=delta;
    /**
     * @brief Assuming we are Querying the 
     * 
     */
    return 0;   
}
int FlyWheelSB::turnOn() {
    float pwr = 127;
    this->pMotorX->move(pwr);
    this->pMotorY->move(pwr);
    return 0;
}

float FlyWheelSB::getRPM() const {
    float rpmI = fabs(this->pMotorX->get_actual_velocity());
    float rpmJ = fabs(this->pMotorY->get_actual_velocity());
    return (0.5 *(rpmI + rpmJ));
}

int FlyWheelSB::reset() {
    this->shotComplete = false;
    this->toggled = false;
    this->engagedOne = false;
    this->cntNoVal = 0;
    this->sumTime = 0;
    if(!loaded) this->currentPos = 0;
    this->pMotorX->move(0);
    this->pMotorY->move(0);
    this->pMotorX->tare_position();
    this->pMotorY->tare_position();
    this->pMag->reset();
    return 0;
}
bool FlyWheelSB::shotCompleted() {
    return this->shotComplete;
}
