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
    ConfigurationParser* configParser;

    public:

    RoboController() {}
    ~RoboController() {}

 
    /* Setter Methods */
    // void setObjHandler(ObjectHandler* ObjectHandler) {
    //     this->objHandler = ObjectHandler;
    // }
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
    // ObjectHandler* getObjHandler() {
    //     return this->objHandler;
    // }
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
    void Run();
    void Stop();
};

void TerriBull::RoboController::Init() {
    /* TBD                                          <-Fix this file path-> */
    pros::lcd::set_text(0,"Entering Init");
    this->configParser = new ConfigurationParser("/usd/configuration.json", "Shooter_Big_Bot");
    if ( this->configParser->success()) {
        /* Init Mech Sys */
        this->system = this->configParser->getMechanicalSystemConfig();
        if (!this->configParser->success()) {
            pros::lcd::set_text(0,"Parsing Error: "+::std::to_string(configParser->getErrCode()) );
            logger.logError(("Configuration Parsing Failed on loading in System, Error Code: "+ ::std::to_string(configParser->getErrCode())));
            // exit(1);
        }
        /* Init Task Manager */
        this->taskManager = new TaskManager(); /* TODO: Needs TaskManager::Init() */

        /* Init Serial Controller */
        this->serialController = new SerialController(); /* TODO: Needs TaskManager::Init() */

        /* Init Object Handler */
        // this->objHandler = new ObjectHandler(); /* TODO: ObjHandler Class Needs serious Update */

        // this->taskManager->addTaskSet(
        //     new TaskList({
        //         new DriveTask(Vector2::cartesianToVector2((this->system->getPosition()).x+50, (this->system->getPosition()).y));
        //     })
        // );

    }
    else {
        pros::lcd::set_text(0,"Parsing Error: "+::std::to_string(configParser->getErrCode()) );
        logger.logError(("Configuration Parsing Failed on initalization, Error Code: "+ ::std::to_string(configParser->getErrCode())));
        // exit(1);
    }
}

void TerriBull::RoboController::Run() {
    // this->taskManager->run();
    pros::lcd::set_text(0,"Parsing Status: "+::std::to_string(configParser->getErrCode()) );
    // this->serialController->update();
    // this->objHandler->update();

    int yInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int xInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_LEFT_X);
    if (abs(yInput) < 5) yInput = 0;
    if (abs(xInput)<5) xInput = 0;
    if (yInput != 0 || xInput != 0) {
        pros::lcd::set_text(4,"Translation");
        Vector2 currentPos = system->getPosition();
      this->system->GoToPosition(currentPos.x+xInput, currentPos.y + yInput);
    } else pros::lcd::set_text(4,"No Translation");
    pros::delay(10);
    
}


#endif
