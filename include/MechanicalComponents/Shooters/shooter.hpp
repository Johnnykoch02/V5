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

    public:
    Shooter() : TerriBull::MechanicalComponent() {}

    virtual int TurnOn(int dir) {} // button is held
    virtual int TurnOff() {}
};     

#endif // SHOOTER_H