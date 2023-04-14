/**
 * @file FlyWheelSB.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Catapult Shooter Mechanism that uses a Zolt-like reflex to shoot the Disc Game Pieces.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#ifndef FLYWHEELSB_H
#define FLYWHEELSB_H

#include "../../shooter.hpp"


class FlyWheelSB : public TerriBull::Shooter {
    protected:
    pros::Motor *pMotorX, *pMotorY;
    pros::ADIDigitalIn *limitSwitch;
    bool engagedOne; float cntNoVal; float sumTime;
    float currentPos;

    int x, y;

    public:
    FlyWheelSB(int _x, bool xReverse, int _y, int yReverse, char limitSwitchPort, int gearSet) : Shooter(gearSet), x(_x), y(_y), engagedOne(false), cntNoVal(0), sumTime(0) {
        this->pType = "FlyWheel-SB";
        this->pMotorX = new pros::Motor(x, xReverse);
        this->pMotorY = new pros::Motor(y, yReverse);
        this->setPID(120, 2.5, 0);
        this->limitSwitch = new pros::ADIDigitalIn(limitSwitchPort);
        this->pMotorX->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
        this->pMotorY->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
    }

    int Shoot(float delta);// button is held
    int Load(float delta);
    int turnOn();
    int reset();
    bool shotCompleted();
};     

#endif // SHOOTER_H
