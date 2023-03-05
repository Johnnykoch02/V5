/**
 * @file roller.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Spinnable device used for manipulating field items in a spin system.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef ROLLER_H
#define ROLLER_H

#include "../../TerriBull/TerriBull.hpp"


class TerriBull::Roller : public TerriBull::MechanicalComponent {
    protected:
    float sumTime;
    float currentPos;
    int maxSpeed;
    public:

    Roller(int maxSpeed) : TerriBull::MechanicalComponent(), maxSpeed(maxSpeed) {}

    virtual int Spin(int direction, float time) = 0;
    virtual int SpinToPos(float pos) = 0;
    virtual void reset() = 0;
};     

#endif // ROLLER_H
