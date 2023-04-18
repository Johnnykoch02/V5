/**
 * @file RollerDuo.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Roller mechanism for Robots to spin wall Game Piece.
 *
 * @version 0.1
 * @date 2023-03-06
 *
 * @copyright Copyright (c) 2022
 *
*/
#include "../../include/MechanicalComponents/Rollers/Configurations/RollerDuo.hpp"
#include <sstream>
#include <iomanip>
int Roller_Duo::Spin(int direction, float time, float delta) {
    this->sumTime+=delta;
    if (this->sumTime < time) {
        std::stringstream s3;
        s3 << std::fixed << ::std::setprecision(1);
        s3 << "Current: "<< this->sumTime << " Goal: " << time;
        pros::lcd::set_text(6, s3.str());
        this->pMotorI->move(direction * this->maxSpeed);
        this->pMotorJ->move(direction * this->maxSpeed);
    } else timeFlag = true;
    this->update();
    return 0;
}

int Roller_Duo::TurnOn(int direction, float pwr) {
    this->pMotorI->move(direction * pwr);
    this->pMotorJ->move(direction * pwr);
    return 0;
}

float Roller_Duo::getRPM() const {
    float rpmI = this->pMotorI->get_actual_velocity();
    float rpmJ = this->pMotorJ->get_actual_velocity();
    return 0.5 *(rpmI + rpmJ);
}

int Roller_Duo::SpinToPos(float pos) {
    this->currentError = pos - this->currentPos;
    this->sumError += this->currentError;
    float pwr = kP*this->currentError + kI*this->sumError + kD*this->dError();
    pros::lcd::set_text(2, "Spinning with " + std::to_string(pos));
    this->pMotorI->move(pwr);
    this->pMotorJ->move(pwr);
    this->update();
    return 0;
}

void Roller_Duo::reset() {
    this->timeFlag = false;
    this->sumTime = 0;
    this->sumError = 0;
    this->previousError = 0;
    this->pMotorI->move(0);
    this->pMotorJ->move(0);
}

void Roller_Duo::update() {
    float dPos = (this->pMotorI->get_position() + this->pMotorJ->get_position()) / 2;
    this->currentPos += 100 * dPos * (2*PI) / ENCODER_UNIT[this->gearSet];
    this->pMotorI->tare_position();
}
