/**
 * @file DanTheMan.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Bull Two Config for Driver: Dan is in fact, the man.
 *
 * @version 0.1
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DANTHEMAN_H
#define DANTHEMAN_H

#include "../InputController.hpp"

class DanTheMan : public TerriBull::InputController {
    public:
    DanTheMan(TerriBull::RoboController* RoboController, int deadzone) : TerriBull::InputController(RoboController, deadzone) {}
    void Init();
    void Update(float delta);
};

#endif
