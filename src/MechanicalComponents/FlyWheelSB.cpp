/**
 * @file FlyWheelSB.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Catapult Shooter Mechanism that uses a Zolt-like reflex to shoot the Disc Game Pieces.
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
    pros::lcd::set_text(6, ""+ std::to_string(this->sumTime));
    if (!this->shotComplete) {
        if (this->sumTime > 0) {
            if (this->limitSwitch->get_new_press()) {
                this->engagedOne = true;
                this->cntNoVal = 0;
            }
            this->pMotorX->move(127);
            this->pMotorY->move(127);
            if (this->engagedOne) {
                pros::lcd::set_text(6, ""+ std::to_string(this->limitSwitch->get_value()));
                if (this->limitSwitch->get_value() == 0) this->cntNoVal+=delta;
                // else this->cntNoVal = 0;
                shotComplete = this->cntNoVal > 0.2;
                if (shotComplete){ this->loaded = false; this->toggled = false;}
            }
        }     
    }
    return 0;
}

int FlyWheelSB::Load(float delta) {
    toggled = true;
    this->sumTime+=delta;
    this->currentError = 0.3 - this->currentPos;
    float pwr = 120;
    this->pMotorX->move(pwr);
    this->pMotorY->move(pwr);
    this->loaded = fabs(this->currentError) <  0.1 || this->limitSwitch->get_value();
    
    pros::lcd::set_text(4, ""+std::to_string(this->currentError));
    if (this->loaded) {
        this->pMotorX->move(0);
        this->pMotorY->move(0);
        this->toggled = false;
    }
    if (this->pMotorX->get_torque() > 0.15) {
        float dP = (this->pMotorX->get_position() + this->pMotorY->get_position()) / (2 * ENCODER_UNIT[this->gearSet]);
        this->currentPos +=dP;
        this->pMotorX->tare_position();
        this->pMotorY->tare_position();
    }
    return 0;   
}
int FlyWheelSB::turnOn() {
    float pwr = 120;
    this->pMotorX->move(pwr);
    this->pMotorY->move(pwr);
    return 0;
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
    return 0;
}
bool FlyWheelSB::shotCompleted() {
    return this->shotComplete;
}
