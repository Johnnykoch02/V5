/**
 * @file CatapultZolt.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Catapult Shooter Mechanism that uses a Zolt-like reflex to shoot the Disc Game Pieces.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#ifndef CATAPULTZOLT_H
#define CATAPULTZOLT_H

#include "../../shooter.hpp"


class CatapultZolt : public TerriBull::Shooter {
    protected:
    pros::Motor *pMotorX, *pMotorY;
    pros::ADIDigitalIn *limitSwitch;
    bool engagedOne; float cntNoVal; float sumTime;
    float currentPos;

    int x, y;

    public:
    CatapultZolt(int _x, bool xReverse, int _y, int yReverse, char limitSwitchPort, int gearSet) : Shooter(gearSet), x(_x), y(_y), engagedOne(false), cntNoVal(0), sumTime(0) {
        this->pType = "Catapult-Zolt";
        this->pMotorX = new pros::Motor(x, xReverse);
        this->pMotorY = new pros::Motor(y, yReverse);
        this->setPID(120, 2.5, 0);
        this->limitSwitch = new pros::ADIDigitalIn(limitSwitchPort);
        this->pMotorX->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
        this->pMotorY->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
        this->pMotorRefs = (pros::Motor**) malloc(sizeof(pros::Motor*)*2);
        this->pMotorRefs[0] = pMotorX;
        this->pMotorRefs[1] = pMotorY;
        this->motorRefs = new MechanicalComponent::MotorRefs {
        this->pType, this->pMotorRefs, 2
      };
    }

    int Shoot(float delta, void* args);// button is held
    int Load(float delta, void* args);
    float getRPM();
    int turnOn();
    int reset();
    bool shotCompleted();
    // void* ConstructUpdateArgs(void);
    int UpdateInternalState(void* args) {return 0;}
};     

#endif // SHOOTER_H
