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
#include "../../include/TerriBull/lib/GameObjects/SpinUp/Disk.hpp"
#include "../../include/TerriBull/lib/GameObjects/SpinUp/FieldRoller.hpp"
#include "../../include/TerriBull/lib/GameObjects/SpinUp/Goal.hpp"
#include <sstream>
#include <iomanip>
void RoboController::setObjHandler(ObjectHandler* objHandler) {
    this->objHandler = objHandler;
}
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
ObjectHandler* RoboController::getObjHandler() {
    return this->objHandler;
}
TaskManager* RoboController::getTaskManager() {
    return this->taskManager;
}
MechanicalSystem* RoboController::getSystem() {
    return this->system;
}
SerialController* RoboController::getSerialController() {
    return this->serialController;
}

void RoboController::runSerialTask(void* args) {
    this->serialController->update();
}
void RoboController::runMainTask(void* args) {
    this->Run();
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
        /* THIS VAR WILL PUT THE SYSTEM INTO DEBUG MODE */
        this->pDebug = false; //false;

        /* Init Task Manager */
        this->taskManager = new TaskManager();
        /* Init Serial Controller */
        this->serialController = new SerialController(this); /* TODO: Needs SerialController::Init() */
        this->serialController->RegisterCallback("set_disk_obj", (SerialController::PacketCallback)SetDiskObjectCallback);
        this->serialController->RegisterCallback("get_disk_obj", (SerialController::PacketCallback)GetDiskObjectCallback);
        this->serialController->RegisterCallback("set_goal_obj", (SerialController::PacketCallback)SetGoalObjectCallback);
        this->serialController->RegisterCallback("get_goal_obj", (SerialController::PacketCallback)GetGoalObjectCallback);
        this->serialController->RegisterCallback("set_roller_obj", (SerialController::PacketCallback)SetRollerObjectCallback);
        this->serialController->RegisterCallback("get_roller_obj", (SerialController::PacketCallback)GetRollerObjectCallback);
        this->serialController->RegisterCallback("get_pos", (SerialController::PacketCallback)GetPositionCallback);
        this->serialController->RegisterCallback("set_pos", (SerialController::PacketCallback)SetPositionCallback);
        this->serialController->RegisterCallback("go_to_pos", (SerialController::PacketCallback)GoToPositionCallback);
        this->serialController->RegisterCallback("go_to_pos_dx_dy", (SerialController::PacketCallback)GoToPositionDxDyCallback);
        this->serialController->RegisterCallback("go_to_pos_dr_dtheta", (SerialController::PacketCallback)GoToPositionDRDThetaCallback);
        this->serialController->RegisterCallback("go_to_obj", (SerialController::PacketCallback)GoToObjectCallback);
        this->serialController->RegisterCallback("turn_to_angle", (SerialController::PacketCallback)TurnToAngleCallback);
        this->serialController->RegisterCallback("spin_roller", (SerialController::PacketCallback)SpinRollerCallback);
        this->serialController->RegisterCallback("shoot_disk", (SerialController::PacketCallback)ShootDiskCallback);
        this->serialController->RegisterCallback("load_shooter", (SerialController::PacketCallback)LoadShooterCallback);
        if(!this->pDebug) this->serialController->ExchangeTags();
        // /* Init Object Handler */
        // this->objHandler = new ObjectHandler(); /* TODO: ObjHandler Class Needs serious Update */

        /* TASKS SECTION */
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::VariableTask(this->system->getDrive()->getRefMaxSpeed(), new float(70), VariableTask::FLOAT, this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(1000, this->getSystem())}));
        this->taskManager->addTaskSet(new TaskList({new TerriBull::IntakeTask(1, IntakeTask::ON, this->getSystem())}));
        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        this->taskManager->addTaskSet(new TaskList({new TerriBull::RollerTask(1000,this->getSystem())}));
        this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        this->taskManager->addTaskSet(
            new TaskList({{
                TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, -17.0), true, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
                // new TerriBull::RollerTask(0.75, 1, this->getSystem())
            }})
        );

        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));

        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(-6.8, 0), true, TerriBull::DriveTask::ORIENTATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));

        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(-25, 0), true, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );

        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        // // this->taskManager->addTaskSet(new TaskList({TerriBull::RollerTask::DynamicInitialize(this->system->getRoller()->posPtr(), -1, this->getSystem() )}));
        // this->taskManager->addTaskSet(new TaskList({TerriBull::RollerTask::DynamicInitialize(this->system->getRoller()->posPtr(), -300, this->getSystem() )}));
        
        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(24, 0), false, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));


        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, 3), true, TerriBull::DriveTask::ORIENTATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));

        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, 20), false, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );

        
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({TerriBull::RollerTask::DynamicInitialize(this->system->getRoller()->posPtr(), -300, this->getSystem() )}));

        //   this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, -12), false, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );

        // // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(1, 1), false, TerriBull::DriveTask::ORIENTATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(-12, -12), true, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );

        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, 1), false, TerriBull::DriveTask::ORIENTATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
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
    this->updateTime();
    this->system->update(this->delta());
    if (pros::competition::is_autonomous()) { /*TODO: Or engaged Autonomous Control */
        this->taskManager->run(this->delta());
    } else {
        if(!this->pDebug) this->inputController->Update(this->delta());
        else {
            float voltages[] = {0, 0, 0, 0, 0, 0};
            this->system->getDrive()->setVoltage(voltages);
            if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
                this->taskManager->run(this->delta());
            }
        }
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



/* Callback Methods */
/**
 * @brief Set the Disk Object Callback object
 * DATA: [int id, float x, float y]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void SetDiskObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) { //[Jetson -> Brain] (id, x, y)
    char id = (char) (int) SerialController::DeserializeNumber(array, &start_index);
    float x = SerialController::DeserializeNumber(array, &start_index);
    float y = SerialController::DeserializeNumber(array, &start_index);
    /*Update Sys*/
    void* update_args = Disk::ConstructUpdateArgs(x, y);
    robot->getObjHandler()->update(GameObject::ROLLER, id, update_args);
    delete static_cast<Disk::UpdateArgs*>(update_args);
}
/**
 * @brief Get the Disk Object Callback object
 * DATA: [int id]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void GetDiskObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) { //[Brain -> Jetson] (id)
    /* Parse for ID */
}
/**
 * @brief Set the Roller Object Callback object
 * DATA: [int id, float x, float y, int color, bool is_in_contact(int)]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void SetRollerObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) { //[Jetson -> Brain] (id, x, y)
    char id = (char) (int) SerialController::DeserializeNumber(array, &start_index);
    float x = SerialController::DeserializeNumber(array, &start_index);
    float y = SerialController::DeserializeNumber(array, &start_index);
    int color = (int) SerialController::DeserializeNumber(array, &start_index);
    bool isInContact = (bool) SerialController::DeserializeNumber(array, &start_index);
    /*Update Sys*/
    void* update_args = FieldRoller::ConstructUpdateArgs(x, y, color, isInContact);
    robot->getObjHandler()->update(GameObject::ROLLER, id, update_args);
    delete static_cast<FieldRoller::UpdateArgs*>(update_args);
}
/**
 * @brief Get the Roller Object Callback object
 * DATA: [int id]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void GetRollerObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) { //[Brain -> Jetson] (id)
    /* Send Request*/
}
/**
 * @brief Set the Goal Object Callback object
 * DATA: [int id, float x, float y, d_theta]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void SetGoalObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) { //[Jetson -> Brain] (id, x, y, color, in_contact)
    char id = (char) (int) SerialController::DeserializeNumber(array, &start_index);
    float x = SerialController::DeserializeNumber(array, &start_index);
    float y = SerialController::DeserializeNumber(array, &start_index);
    float dTheta = SerialController::DeserializeNumber(array, &start_index);
    /*Update Sys*/
    void* update_args = Goal::ConstructUpdateArgs(x, y, dTheta);
    robot->getObjHandler()->update(GameObject::ROLLER, id, update_args);
    delete static_cast<Goal::UpdateArgs*>(update_args);
}
/**
 * @brief Get the Goal Object Callback object
 * DATA: [int id]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void GetGoalObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {

}
/**
 * @brief Set the Position Callback object
 * DATA: []
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void SetPositionCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    
}
/**
 * @brief Get the Position Callback object -> x, y, theta
 * DATA: []
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void GetPositionCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    std::stringstream s3;
    Vector2* pos = robot->getSystem()->getPosition();
    float theta = robot->getSystem()->getAngle();
    s3 << (unsigned char) robot->getSerialController()->GetCallbackIndex("get_pos");
    s3 << SerialController::SerializeNumber(pos->x);
    s3 << SerialController::SerializeNumber(pos->y);
    s3 << SerialController::SerializeNumber(theta);
    robot->getSerialController()->SendData(s3.str());
}

/* API Callbacks */
/**
 * @brief 
 * DATA: [float x, float y, bool reversed] <- we could implement as Dx, Dy
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void GoToPositionCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float x = SerialController::DeserializeNumber(array, &start_index);
    float y = SerialController::DeserializeNumber(array, &start_index);
    bool reversed = (bool) SerialController::DeserializeNumber(array, &start_index);
    Vector2* pos = Vector2::cartesianToVector2(x, y);
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::DriveTask(*pos, 420, reversed, TerriBull::DriveTask::ORIENTATION, robot->getSystem()),
    }));
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::DriveTask(*pos, 420, reversed, TerriBull::DriveTask::TRANSLATION, robot->getSystem()),
    }));
    delete pos;
}
/**
 * @brief 
 * DATA: [float x, float y, bool reversed]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void GoToPositionDxDyCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float x = SerialController::DeserializeNumber(array, &start_index);
    float y = SerialController::DeserializeNumber(array, &start_index);
    bool reversed = (bool) SerialController::DeserializeNumber(array, &start_index);
    Vector2* pos = Vector2::cartesianToVector2(x, y);
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        TerriBull::DriveTask::DynamicInitialize(pos, reversed, TerriBull::DriveTask::ORIENTATION, robot->getSystem()),
    }));
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        TerriBull::DriveTask::DynamicInitialize(pos, reversed, TerriBull::DriveTask::TRANSLATION, robot->getSystem()),
    }));
    delete pos;
}
/**
 * @brief 
 * DATA: [float r, float d_theta, bool reversed]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void GoToPositionDRDThetaCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float r = SerialController::DeserializeNumber(array, &start_index);
    float dtheta = SerialController::DeserializeNumber(array, &start_index);
    bool reversed = (bool) SerialController::DeserializeNumber(array, &start_index);
    Vector2* pos = Vector2::polarToVector2(r, robot->getSystem()->getAngle()+ dtheta);
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        TerriBull::DriveTask::DynamicInitialize(pos, reversed, TerriBull::DriveTask::ORIENTATION, robot->getSystem()),
    }));
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        TerriBull::DriveTask::DynamicInitialize(pos, reversed, TerriBull::DriveTask::TRANSLATION, robot->getSystem()),
    }));
    delete pos;
}
/**
 * @brief Create a Object Callback object
 * 
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void CreateObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    int id = (int) SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject::Types objectType = (TerriBull::GameObject::Types) (int) SerialController::DeserializeNumber(array, &start_index);
    int x = SerialController::DeserializeNumber(array, &start_index);
    int y = SerialController::DeserializeNumber(array, &start_index);
    GameObject* obj = nullptr;
    Vector2* pos = Vector2::cartesianToVector2(x, y);
    switch (objectType) {
        case GameObject::Types::DISK: {
            obj = new Disk(pos, id);
            break;
        }
        case GameObject::Types::ROLLER: {
            obj = new FieldRoller(pos, id);
            break;
        }
        case GameObject::Types::GOAL: {
            obj = new Goal(pos, id);
            break;
        }
        default:
            break;
    }       
}
/**
 * @brief Goes to an Object based on a identifier and type.
 * DATA: [char OBJECT_ID, int OBJECT_TYPE, bool reversed]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 * NOTE:  Note: IT IS NOT RECOMMENDED TO GO TO OBJECTS IN THE REVERSE DIRECTION
 */
void GoToObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    int id = (int) SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject::Types objectType = (TerriBull::GameObject::Types) (int) SerialController::DeserializeNumber(array, &start_index);
    bool reversed = (bool) SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject* obj = robot->getObjHandler()->query(objectType, id);    
    if (obj == nullptr) { return; } /* TODO: Needs ERRNO-like Message Passing */
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        TerriBull::DriveTask::GoToObject(obj, reversed, robot->getSystem()),
    }));
}
/**
 * @brief 
 * DATA: [float angle] <- we could implement as Dtheta
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void TurnToAngleCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float angle = (int) SerialController::DeserializeNumber(array, &start_index);
    bool reversed = (bool) SerialController::DeserializeNumber(array, &start_index);
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::DriveTask(nullptr, angle, reversed, DriveTask::DriveType::ORIENTATION, robot->getSystem()),
    }));
}
/**
 * @brief 
 * DATA: []
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void SpinRollerCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    int dir = SerialController::DeserializeNumber(array, &start_index);
    robot->getSystem()->SpinRollerFor(dir, 1000); /*TODO: Change such that there is a Power Parameter */
}
/**
 * @brief 
 * DATA: [int GOAL_ID]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void ShootDiskCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    int goal_id = SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject* obj = robot->getObjHandler()->query(GameObject::Types::GOAL, goal_id);    
    if (obj == nullptr) { return; } /* TODO: Needs ERRNO-like Message Passing */
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::ShooterTask(ShooterTask::SHOOT,robot->getSystem()),
    }));
}
/**
 * @brief 
 * DATA: [int DISK_ID]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void LoadShooterCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    int disk_id = SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject* obj = robot->getObjHandler()->query(GameObject::Types::DISK, disk_id);    
    if (obj == nullptr) { return; } /* TODO: Needs ERRNO-like Message Passing */
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::ShooterTask(ShooterTask::SHOOT,robot->getSystem()),
    }));
}
/* Task Management Callbacks */
/**
 * @brief 
 * DATA: []
 * @param robot 
 * @param array 
 * @param start 
 * @param length 
 */
void ClearTasksCallback(TerriBull::RoboController* robot, char * array, int start, int length) {
    robot->getTaskManager()->ClearAllTasks();
}
/* Other Callbacks */
void TagExchangeCallback(TerriBull::RoboController* robot, char * array, int start, int length) {
    robot->getSerialController()->updateExchangeTags();
}