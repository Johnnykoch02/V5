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
    /* Systems */
    ObjectHandler* objHandler;
    TaskManager* taskManager;
    MechanicalSystem* system;
    SerialController* serialController;
    ConfigurationParser configParser;

    public:

    RoboController();

    /* Setter Methods */
    void setObjHandler(ObjectHandler* ObjectHandler) {
        this->objHandler = ObjectHandler;
    }
    void setTaskManager(TaskManager* taskManager) {
        this->taskManager = taskManager;
    }
    void setSystem(MechanicalSystem* system) {
        this->system = system;
    }
    void setSerialController(SerialController* serialController) {
        this->serialController = serialController;
    }
    /* Getter Methods */
    ObjectHandler* getObjHandler() {
        return this->objHandler;
    }
    TaskManager* getTaskManager() {
        return this->taskManager;
    }
    MechanicalSystem* getSystem() {
        return this->system;
    }
    SerialController* getSerialController() {
        return this->serialController;
    }
    /* Operational Methods */
    void Init();
    void run();
    void stop();
};





#endif