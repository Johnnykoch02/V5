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
    if (this->pMag->getMagazineCount() <= 0) {
        this->shotComplete = true;
        return 0;
    } 
    /**
     * @brief Wait for the FlyWheel to Spin up, then start the Shooting.
     */
    this->pToggled = true;
    this->turnOn();
    this->pMag->update(delta);
    pros::lcd::set_text(0,"Mag Cnt: "+to_string(this->pMag->getMagazineCount()));
    pros::lcd::set_text(1,"RPM: "+to_string(this->getRPM()));
    if (!(this->getRPM() > MIN(0.8*this->targetRPM, 480))) { /*TODO*/ /*480 is the max stable RPM*/
        return 0;
    }
    
    this->pSystem->TurnOnIntake(0.75);
    if (this->pMag->getDecToggle()) {
        this->toggled = true;
    }
    if (this->toggled) {
        this->sumTime+=delta;
    }
    this->shotComplete = (this->sumTime >= 1.5);
    if (this->shotComplete) {
        /* Ensure that the Mag has been emptied */
        if (this->pMag->getMagazineCount() > 0) {
            this->pMag->reset();
            this->sumTime = 0;
            this->shotComplete = false;
        }
        this->pSystem->TurnOffIntake();
    }
    return 0;
}

int FlyWheelSB::Load(float delta, void* args) {
    this->loaded = false;
    pros::lcd::set_text(0,"Mag Cnt: "+to_string(this->pMag->getMagazineCount()));
    this->pToggled = true;
    this->pSystem->TurnOnIntake(-1);
    this->sumTime+=delta;
    /**
     * @brief Assuming we are Querying the Mechanical System for an Object to Load
     */
    GameObject* obj = this->pSystem->getTargetObject();
    // if this->pSystem->Intake
    if (!this->pMag->getDecToggle()) this->pMag->update(delta);
    else {
        this->toggled = true;
        this->sumTime+=delta;
    }
    if (!this->toggled) { /* If we Havent toggled the mag then we should slowly drive forward */
        if (obj!= nullptr) {
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
    this->loaded = this->pMag->getMagazineCount() > 0 && this->sumTime > 2.5;
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
