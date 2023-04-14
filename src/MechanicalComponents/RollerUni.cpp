/**
 * @file RollerUni.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Roller mechanism for Robots to spin wall Game Piece.
 *
 * @version 0.1
 * @date 2023-03-06
 *
 * @copyright Copyright (c) 2022
 *
*/
#include "../../include/MechanicalComponents/Rollers/Configurations/RollerUni.hpp"
int Roller_Uni::Spin(int direction, float time, float delta) {
    this->sumTime+=delta;
    if (this->sumTime < time) {
        this->pMotorI->move(direction * this->maxSpeed);
    } else timeFlag = true;
    this->update();
    return 0;
}

int Roller_Uni::SpinToPos(float pos) {/* TODO: Fix this */
    this->currentError = pos - this->currentPos;
    this->sumError += this->currentError;
    float pwr = kP*this->currentError + kI*this->sumError + kD*this->dError();
    this->pMotorI->move(pwr);
    this->update();
    return 0;
}

void Roller_Uni::reset() {
    this->timeFlag = false;
    this->sumTime = 0;
    this->sumError = 0;
    this->previousError = 0;
    this->pMotorI->move(0);
}

void Roller_Uni::update() {
    float dPos = this->pMotorI->get_position();
    this->currentPos += 25 * dPos * (2*PI) / ENCODER_UNIT[this->gearSet];
    this->pMotorI->tare_position();
}
