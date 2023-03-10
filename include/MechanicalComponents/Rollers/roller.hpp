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
    bool timeFlag;
    Roller(int maxSpeed, int gearSet) : TerriBull::MechanicalComponent(gearSet), maxSpeed(maxSpeed), timeFlag(false), currentPos(0) {}
    virtual int Spin(int direction, float time, float delta) = 0;
    virtual float* posPtr() final { return &currentPos; }
    virtual float getPos() const final { return currentPos; }    
    virtual int SpinToPos(float pos) = 0;
    virtual void reset() = 0;
    virtual void update() = 0;
};     

#endif // ROLLER_H
