/**
 * @file RoboController.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Master controller of all the subsystems
 * embedded into the BullBot
 *
 * @version 0.1
 * @date 2023-02-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "../../include/Controllers/RoboController/RoboController.hpp"
void RoboController::setTaskManager(TaskManager* taskManager) {
    this->taskManager = taskManager;
}
void RoboController::setSystem(MechanicalSystem* system) {
    this->system = system;
}
void RoboController::setSerialController(SerialController* serialController) {
    this->serialController = serialController;
}
    /* Getter Methods */
    // ObjectHandler* getObjHandler() {
    //     return this->objHandler;
    // }
TaskManager* RoboController::getTaskManager() {
    return this->taskManager;
}
MechanicalSystem* RoboController::getSystem() {
    return this->system;
}
SerialController* RoboController::getSerialController() {
    return this->serialController;
}


void TerriBull::RoboController::Init() {
    /* TBD                                          <-Fix this file path-> */
    this->configParser = new ConfigurationParser("/usd/configuration.json", "Shooter_Big_Bot");
    if ( this->configParser->success()) {
        /* Init Mech Sys */
        this->system = this->configParser->getMechanicalSystemConfig();
        if (!this->configParser->success()) {
            pros::lcd::set_text(0,"Parsing Error: "+::std::to_string(configParser->getErrCode()) );
            logger.logError(("Configuration Parsing Failed on loading in System, Error Code: "+ ::std::to_string(configParser->getErrCode())));
            // exit(1);
        }
        this->inputController = this->configParser->getInputControllerConfig(this);
        if (inputController == nullptr) {
            exit(1);
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
        this->previousTime = pros::millis();
        this->currentTime = pros::millis();
    }
    else {
        pros::lcd::set_text(0,"Parsing Error: "+::std::to_string(configParser->getErrCode()) );
        logger.logError(("Configuration Parsing Failed on initalization, Error Code: "+ ::std::to_string(configParser->getErrCode())));
        // exit(1);
    }
}

void TerriBull::RoboController::Run() {
    // this->taskManager->run();
    this->updateTime();
    // this->serialController->update();
    // this->objHandler->update();
    this->inputController->Update(this->delta());
    pros::delay(10);
}

void TerriBull::RoboController::updateTime() {
    this->previousTime = this->currentTime;
    this->currentTime = pros::millis();
}

float TerriBull::RoboController::delta() {
    return (this->currentTime - this->previousTime) / 1000.0f;
}
