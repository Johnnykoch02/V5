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
int FlyWheelSB::Shoot(float delta, void* args) { /* TODO: Create Target RPM */
    if ( this->pMag->getMagazineCount() <= 0 || this->pSystem->getTargetObject()->type != GameObject::GOAL ) {
        this->shotComplete = true;
        return 0;
    } 
    /**
     * @brief We must first turn to the Desired Angle if the Robot is using Autoshoot Mode.
     * Assume since that we are using Autoshoot Mode, our Mechanical System has the Target Object Loaded in.
     */
    if ( this->pSystem->getIsAutoShoot() && !(this->pToggled) ) {
        /* NOTE: this may cause undesirable changes, ensure that if Auto-Shoot then the Jetson has set the Target Object accordingly */
        Goal* goal = dynamic_cast<Goal*>(this->pSystem->getTargetObject()); 
        float desiredAngle = (float) fmod(this->pSystem->getAngle() + goal->getDTheta() + this->pSystem->getTargetDeltaShootingAngle(), 360.0);
        if ( fabs(TerriBull::GetDTheta(desiredAngle, this->pSystem->getAngle())) < 0.9 ) {
            this->pToggled = true;
            this->pSystem->ResetDrive();
        }
        else { /* Turning to the Desired Angle */
            this->pSystem->TurnToAngle(desiredAngle);
        }
    }

    /**
     * @brief Wait for the FlyWheel to Spin up, then start the Shooting.
     */
    this->pToggled = true;
    this->turnOn();
    this->pMag->update(delta);
    // pros::lcd::set_text(0,"Mag Cnt: "+to_string(this->pMag->getMagazineCount()));
    // pros::lcd::set_text(1,"RPM: "+to_string(this->getRPM()));
    if (!(this->getRPM() > MIN(0.8*this->targetRPM, 480))) { /*TODO*/ /*480 is the max stable RPM*/
        return 0;
    }
    
    this->pSystem->TurnOnIntake(1);
    if ( this->pMag->getDecToggle() ) {
        this->toggled = true;
    }
    if (this->toggled) {
        this->sumTime+=delta;
        /* Ensure that the Mag has been emptied */
        if (this->pMag->getMagazineCount() > 0) {
            this->pMag->reset();
            this->sumTime = 0;
            this->toggled = false;
            this->shotComplete = false;
            this->pSystem->TurnOnIntake(0);
        }
    }
    this->shotComplete = (this->sumTime >= 0.5);
    if (this->shotComplete) {
        this->pSystem->TurnOffIntake();
    }
    return 0;
}

int FlyWheelSB::Load(float delta, void* args) {
    this->loaded = false;
    pros::lcd::set_text(1,"Mag Cnt: "+to_string(this->pMag->getMagazineCount()));
    this->pToggled = true;
    this->pSystem->TurnOnIntake(-1);
    /**
     * @brief Assuming we are Querying the Mechanical System for an Object to Load
     */
    GameObject* obj = this->pSystem->getTargetObject();
    // if this->pSystem->Intake
    bool toggledBeforeUpdate = this->pMag->getIncToggle();
    this->pMag->update(delta);
    bool toggledAfterUpdate = this->pMag->getIncToggle();
    if(toggledBeforeUpdate != toggledAfterUpdate) {
        if (this->toggled) this->sumTime = 0; /* We are intaking an extra disk so we need to allow more time for the process to finish */
        this->toggled = true;
        this->pMag->reset();
    }
    if (this->toggled) {
        this->sumTime+=delta;
    }
    else { /* If we Havent toggled the mag then we should slowly drive forward */
        if (obj!= nullptr && this->pSystem->getTargetObject()->type == GameObject::DISK) {
            Vector2* targetPos = obj->getPos();
            this->pSystem->GoToPosition(targetPos, this->pSystem->getPosition(), false);
            delete targetPos;
        } 
        else {
            float* voltages = new float[this->pSystem->getDrive()->getMotorRefs()->NumMotors];
            for (int i = 0; i < this->pSystem->getDrive()->getMotorRefs()->NumMotors; i++) {
                voltages[i] = 15;
            }
            this->pSystem->getDrive()->setVoltage(voltages);
        }
    }
    this->loaded = this->toggled && this->sumTime > 1.2;
    if (this->loaded)
    { this->pSystem->TurnOffIntake(); this->pSystem->ResetDrive(); this->pMag->reset(); }
    return 0;   
}
int FlyWheelSB::turnOn() {
    this->pToggled = true;
    /* Make this Variable*/
    this->pMotorX->move_velocity(this->targetRPM);
    this->pMotorY->move_velocity(this->targetRPM);
    // this->pMag->update(0);
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
    this->toggled = false;
    this->engagedOne = false;
    this->cntNoVal = 0;
    this->sumTime = 0;
    this->loaded = this->pMag->getMagazineCount() > 0;
    this->pMotorX->move(0);
    this->pMotorY->move(0);
    this->pMotorX->tare_position();
    this->pMotorY->tare_position();
    this->pMag->reset();
    this->targetRPM = 600;

    return 0;
}
bool FlyWheelSB::shotCompleted() {
    return this->shotComplete;
}

void* FlyWheelSB::ConstructUpdateArgs(float targetRPM, float maxSpeed) {
    size_t size = sizeof(FlyWheelSB::UpdateArgs); /* Calculate Size*/
    void* data = malloc(size);
    UpdateArgs args = {targetRPM, maxSpeed};
    memcpy((char*)data, &args, sizeof(size));
    return data;
}

int FlyWheelSB::UpdateInternalState(void* args) {
    if (args == nullptr) {
        return -1;
    }
    UpdateArgs* _args = static_cast<FlyWheelSB::UpdateArgs*>(args);
    this->targetRPM = _args->targetRPM;
    this->maxSpeed = _args->maxSpeed;
    delete _args;
    return 0;
}

Magazine* FlyWheelSB::getMag() const {
    return this->pMag;
}