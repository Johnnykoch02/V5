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
#include "../../TerriBull/lib/ConfigurationParser.hpp"

using namespace TerriBull;
#include <string>


class TerriBull::RoboController {
    private:
    /* Systems */
    // ObjectHandler* objHandler;
    TerriBull::TaskManager* taskManager;
    TerriBull::MechanicalSystem* system;
    TerriBull::SerialController* serialController;
    TerriBull::InputController* inputController;
    ConfigurationParser* configParser;
    uint32_t currentTime, previousTime;

    public:

    RoboController() {}
    ~RoboController() {}

 
    /* Setter Methods */
    // void setObjHandler(ObjectHandler* ObjectHandler) {
    //     this->objHandler = ObjectHandler;
    // }
    void setTaskManager(TaskManager* taskManager);
    void setSystem(MechanicalSystem* system);
    void setSerialController(SerialController* serialController);
    /* Getter Methods */
    // ObjectHandler* getObjHandler() {
    //     return this->objHandler;
    // }
    TaskManager* getTaskManager();
    MechanicalSystem* getSystem();
    SerialController* getSerialController();
    /* Operational Methods */
    void Init();
    void updateTime();
    float delta();
    void Run();
    void Stop();
    int ClearTasks();
};

#endif
