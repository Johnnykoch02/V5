/**
 * @file InputController.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief TerriBull Abstract InputController Class for handling Controller input.
 *
 * @version 0.1
 * @date 2023-02-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H
#include "../../TerriBull/TerriBull.hpp"

class TerriBull::InputController {
    protected:
    RoboController* roboController;
    int deadzone;
    
    public:
        InputController(RoboController* roboController, int deadzone) : roboController(roboController), deadzone(deadzone) {}
        virtual void Init() {}
        virtual void Update(int delta) {}

};


#endif
