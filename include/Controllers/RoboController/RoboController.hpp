/**
 * @file RoboController.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Master controller of all the subsystems
 * embedded into the BullBot
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ROBOCONTROLLER_H
#define ROBOCONTROLLER_H

#include "../../TerriBull/TerriBull.hpp"

using namespace TerriBull;

class RoboController {
    private:
        ObjectHandler* objHandler;
        TaskManager* taskManager;
        MechanicalSystem* system;
        SerialController* serialController;
        // ConfigurationParser configParser;

    public:

    RoboController();

};

#endif