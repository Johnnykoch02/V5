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
    TerriBull::ObjectHandler* objHandler;
    TerriBull::MechanicalSystem* system;
    TerriBull::SerialController* serialController;
    TerriBull::InputController* inputController;
    ConfigurationParser* configParser;
    uint32_t currentTime, previousTime;
    bool pDebug;

    public:

    RoboController() {}
    ~RoboController() {}

 
    /* Setter Methods */
    void setObjHandler(ObjectHandler* objHandler);
    void setTaskManager(TaskManager* taskManager);
    void setSystem(MechanicalSystem* system);
    void setSerialController(SerialController* serialController);
    /* Getter Methods */
    ObjectHandler* getObjHandler();
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

    void runSerialTask(void* args);
    void runMainTask(void* args);

};

#endif


#ifndef __CALLBACK_METHODS__
#define __CALLBACK_METHODS__

#include <string>
#include <vector>
#include <map>
/* Data Callbacks */
extern void SetDiskObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length); //[Jetson -> Brain] (id, x, y)
extern void GetDiskObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length); //[Brain -> Jetson] (id)
extern void SetRollerObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length); //[Jetson -> Brain] (id, x, y)
extern void GetRollerObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length); //[Brain -> Jetson] (id)
extern void SetGoalObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length); //[Jetson -> Brain] (id, x, y, color, in_contact)
extern void GetGoalObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void SetPositionCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void GetPositionCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void UpdateMagCountCallback(TerriBull::RoboController* robot, char * array, int start_index, int length); // [Brain -> Jetson] (count)

/* API Callbacks */
extern void GoToPositionCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void GoToPositionDxDyCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void GoToPositionDRDThetaCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void CreateObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void SetTargetObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void UpdateTargetObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void UpdateTargetObjectDRDThetaCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void GoToObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void TurnToAngleCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void SpinRollerCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void TurnOffRollerCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void ShootDiskCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);
extern void LoadShooterCallback(TerriBull::RoboController* robot, char * array, int start_index, int length);

/* Task Management Callbacks */
extern void ClearTasksCallback(TerriBull::RoboController* robot, char * array, int start, int length);
extern void TaskFinishedCallback(TerriBull::RoboController* robot, char * array, int start, int length);
/* Other Callbacks */
extern void TagExchangeCallback(TerriBull::RoboController* robot, char * array, int start, int length);
extern void SerialTestJetsonToV5Callback(TerriBull::RoboController* robot, char * array, int start, int length);
extern void SerialTestV5ToJetsonCallback(TerriBull::RoboController* robot, char * array, int start, int length);
//recieve status callback, update status
#endif