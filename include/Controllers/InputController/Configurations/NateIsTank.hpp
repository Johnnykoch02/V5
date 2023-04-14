//10.7
/**
 * @file NateIsTank.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Bull One Config for Driver: O' Captian my Captian, Nathaniel
 *
 * @version 0.1
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef NATEISTANK_H
#define NATEISTANK_H

#include "../InputController.hpp"

class NateIsTank : public TerriBull::InputController {
    public:
    NateIsTank(TerriBull::RoboController* RoboController, int deadzone) : TerriBull::InputController(RoboController, deadzone) {}
    void Init();
    void Update(float delta);
};

#endif
