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
        this->pMotorI = new pros::Motor(i, iReverse);
        this->setPID(4.5, 0.01, 0.8);
    }

    int Spin(int direction, float time, float delta);
    int SpinToPos(float pos);
    void reset();
    void update();
};     

#endif
