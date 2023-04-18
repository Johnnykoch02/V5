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
#ifndef ROLLER_UNI_H
#define ROLLER_UNI_H

#include "../roller.hpp"
#include "../../pros/apix.h" 


class Roller_Uni : public TerriBull::Roller {
    protected:
    pros::Motor* pMotorI;

    public:
    Roller_Uni(int i, bool iReverse, int _maxSpeed, int gearSet) : Roller(_maxSpeed, gearSet) {
        this->pType = "Roller-Uni";
        this->pMotorI = new pros::Motor(i, (pros::motor_gearset_e) this->gearSet, iReverse);
        this->pMotorI->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
        this->setPID(4.5, 0, 0.8);
    }

    int Spin(int direction, float time, float delta);
    int TurnOn(int direction, float pwr);
    float getRPM() const;
    int SpinToPos(float pos);
    void reset();
    void update();
};     

#endif
