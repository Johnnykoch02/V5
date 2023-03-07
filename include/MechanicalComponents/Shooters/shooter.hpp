/**
 * @file shooter.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Intake mechanism for Robots to take in Game Objects.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#ifndef SHOOTER_H
#define SHOOTER_H

#include "../../TerriBull/TerriBull.hpp"


class TerriBull::Shooter : public TerriBull::MechanicalComponent {
    protected:
    bool toggled;
    bool shotComplete;

    public:
    Shooter(int gearSet) : TerriBull::MechanicalComponent(gearSet), toggled(false), shotComplete(false) {}

    virtual int Shoot(float delta) = 0;// button is held
    virtual int turnOn() = 0;
    virtual int reset() = 0;
    virtual bool shotCompleted() = 0;
    virtual bool isToggled() const final { return toggled; }
};     

#endif // SHOOTER_H
