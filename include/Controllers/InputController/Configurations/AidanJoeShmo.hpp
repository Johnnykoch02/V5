/**
 * @file AidanJoeShmo.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Bull Two Config for Driver: Aidan aka. Joe Shmo.
 *
 * @version 0.1
 * @date 2023-02-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef AIDANJOE_SHMO_H
#define AIDANJOE_SHMO_H

#include "../InputController.hpp"

class AidanJoeShmo : public TerriBull::InputController {
    public:
    AidanJoeShmo(TerriBull::RoboController* RoboController, int deadzone) : TerriBull::InputController(RoboController, deadzone) {}
    void Init();
    void Update(float delta);
};

#endif
