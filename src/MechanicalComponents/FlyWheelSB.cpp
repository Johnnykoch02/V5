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
    this->pToggled = true;
    this->sumTime+=delta;
    this->pMag->update(delta);
    return 0;
}

int FlyWheelSB::Load(float delta, void* args) {
    this->pToggled = true;
    this->sumTime+=delta;
    /**
     * @brief Assuming we are Querying the Mechanical System
     */
    GameObject* obj = this->pSystem->getTargetObject();
    // if this->pSystem->Intake
    Vector2* targetPos = nullptr;
    if (obj!= nullptr) {
        targetPos = obj->getPos();
    } 
    else {
        Vector2* posUnit = this->pSystem->getPosition()->unit();
        targetPos = *(this->pSystem->getPosition()) + *posUnit; /* Have it move forward */
        delete posUnit;
    }
    this->pMag->update(delta);
    
    return 0;   
}
int FlyWheelSB::turnOn() {
    this->pToggled = true;
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
    this->pToggled = false;
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
