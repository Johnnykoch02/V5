/**
 * @file RyanIsTank.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Bull One Config for Driver: The Man, the Myth, the Ryan
 *
 * @version 0.1
 * @date 2023-02-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef RYANISTANK_H
#define RYANISTANK_H

#include "../InputController.hpp"

class RyanIsTank : public TerriBull::InputController {
    public:
    RyanIsTank(TerriBull::RoboController* RoboController, int deadzone) : TerriBull::InputController(RoboController, deadzone) {}
    void Init();
    void Update(float delta);
};

#endif
