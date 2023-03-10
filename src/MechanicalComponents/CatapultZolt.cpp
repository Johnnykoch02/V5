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
    this->sumTime+=delta;
    pros::lcd::set_text(6, ""+ std::to_string(this->sumTime));
    if (!this->shotComplete) {
        if (this->sumTime > 1.5) {
            if (this->limitSwitch->get_new_press()) {
                this->engagedOne = true;
                this->cntNoVal = 0;
            }
            this->pMotorX->move(127);
            this->pMotorY->move(127);
            if (this->engagedOne) {
                pros::lcd::set_text(6, ""+ std::to_string(this->limitSwitch->get_value()));
                if (this->limitSwitch->get_value() == 0) this->cntNoVal+=delta;
                else this->cntNoVal = 0;
                shotComplete = this->cntNoVal > 0.5;
            }
        }
        else {
            this->pMotorX->move(-100);
            this->pMotorY->move(-100);   
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
    this->engagedOne = false;
    this->cntNoVal = 0;
    this->sumTime = 0;
    this->pMotorX->move(0);
    this->pMotorY->move(0);
    return 0;
}
bool CatapultZolt::shotCompleted() {
    return this->shotComplete;
}