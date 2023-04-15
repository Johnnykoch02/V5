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
    /*                                                  * Config File *   * Config Type *       */
    this->configParser = new ConfigurationParser("/usd/configuration.json", "TinyBull");
    if ( this->configParser->success()) {
        /* Init Mech Sys */
        this->system = this->configParser->getMechanicalSystemConfig();
        if (!this->configParser->success()) {
            pros::lcd::set_text(0,"Parsing Error: "+::std::to_string(configParser->getErrCode()) );
            logger.logError(("Configuration Parsing Failed on loading in System, Error Code: "+ ::std::to_string(configParser->getErrCode())));
            // exit(1);
        }
        this->system->setMotherSystem(this);
        this->inputController = this->configParser->getInputControllerConfig(this);
        if (inputController == nullptr) {
            // exit(1);
        }
        /* Init Task Manager */
        this->taskManager = new TaskManager();
        /* Init Serial Controller */
        this->serialController = new SerialController(); /* TODO: Needs SerialController::Init() */
        /* Init Object Handler */
        // this->objHandler = new ObjectHandler(); /* TODO: ObjHandler Class Needs serious Update */

        /* TASKS SECTION */
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::VariableTask(this->system->getDrive()->getRefMaxSpeed(), new float(70), VariableTask::FLOAT, this->getSystem())}));
        
        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, -17.0), true, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );

        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));

        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(-6.8, 0), true, TerriBull::DriveTask::ORIENTATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );
        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));

        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(-70, 0), false, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );

        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({TerriBull::RollerTask::DynamicInitialize(this->system->getRoller()->posPtr(), -1, this->getSystem() )}));
        this->taskManager->addTaskSet(new TaskList({TerriBull::RollerTask::DynamicInitialize(this->system->getRoller()->posPtr(), -300, this->getSystem() )}));
        
        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(24, 0), false, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );
        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));


        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, 3), true, TerriBull::DriveTask::ORIENTATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );
        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));

        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, 20), false, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );

        
        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        this->taskManager->addTaskSet(new TaskList({TerriBull::RollerTask::DynamicInitialize(this->system->getRoller()->posPtr(), -300, this->getSystem() )}));

          this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, -12), false, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );

        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(1, 1), false, TerriBull::DriveTask::ORIENTATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );
        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(-12, -12), true, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );

        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, 1), false, TerriBull::DriveTask::ORIENTATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );

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
    this->updateTime();
    this->system->update(this->delta());
    if (pros::competition::is_autonomous()) { /*TODO: Or engaged Autonomous Control */
        this->taskManager->run(this->delta());
    } else {
        this->inputController->Update(this->delta());
    }
    
    // this->serialController->update();
    // this->objHandler->update();

    pros::delay(25);
}

void TerriBull::RoboController::updateTime() {
    this->previousTime = this->currentTime;
    this->currentTime = pros::millis();
}

float TerriBull::RoboController::delta() {
    return (this->currentTime - this->previousTime) / 1000.0f;
}

int TerriBull::RoboController::ClearTasks() {
    this->taskManager->ClearAllTasks();
    return 0;
}
