/**
 * @file RollerDuo.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Roller mechanism for Robots to spin wall Game Piece.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#include "../../include/MechanicalComponents/Rollers/Configurations/RollerDuo.hpp"
int Roller_Duo::Spin(int direction, float time, float delta) {
    this->sumTime+=delta;
    if (!this->sumTime>=time) {
        this->pMotorI->move(direction * this->maxSpeed);
        this->pMotorJ->move(direction * this->maxSpeed);
    }
    this->update();
    return 0;
}

int Roller_Duo::SpinToPos(float pos) {
    this->currentError = pos - this->currentPos;
    this->sumError += this->currentError;
    float pwr = kP*this->currentError + kI*this->sumError + kD*this->dError();
    this->pMotorI->move(pwr);
    this->pMotorJ->move(pwr);
    this->update();
    return 0;
}

void Roller_Duo::reset() {
    this->sumTime = 0;
    this->sumError = 0;
    this->previousError = 0;
    this->pMotorI->move(0);
    this->pMotorJ->move(0);
}

void Roller_Duo::update() {
    float dPos = (this->pMotorI->get_position() + this->pMotorJ->get_position()) / 2;
    this->currentPos += dPos * (2*PI) / ENCODER_UNIT[this->gearSet];
    this->pMotorI->tare_position();
}
