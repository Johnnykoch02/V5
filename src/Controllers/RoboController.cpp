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
#include "../../include/MechanicalComponents/Shooters/Configurations/FlyWheel/FlyWheelSB.hpp"
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
    // this->serialController->update(this->delta());
}
void RoboController::runMainTask(void* args) {
    // this->Run();
}


void TerriBull::RoboController::Init() {
    /*                                                  * Config File *   * Config Type *       */
    this->configParser = new ConfigurationParser("/usd/configuration.json", "TinyBull");
    if ( this->configParser->success()) {
        /* Init Mech Sys */
        this->system = this->configParser->getMechanicalSystemConfig();
        if (!this->configParser->success()) {
            //pros::lcd::set_text(0,"Parsing Error: "+::std::to_string(configParser->getErrCode()) );
            logger.logError(("Configuration Parsing Failed on loading in System, Error Code: "+ ::std::to_string(configParser->getErrCode())));
            // exit(1);
        }
        this->system->setMotherSystem(this);
        this->inputController = this->configParser->getInputControllerConfig(this);
        if (inputController == nullptr) {
            // exit(1);
        }
        /* THIS VAR WILL PUT THE SYSTEM INTO DEBUG MODE */
        this->pDebug = true; //false;
        this->pUseSerial = true;
        /* Init Task Manager */
        this->taskManager = new TaskManager();
        /* Init Serial Controller */
        this->serialController = new SerialController(this); /* TODO: Needs SerialController::Init() */
        if (this->pUseSerial) {
            if (this->pDebug) {
                this->serialController->RegisterCallback("serial_test_v5_to_jetson", (SerialController::PacketCallback)SerialTestV5ToJetsonCallback);
                this->serialController->RegisterCallback("serial_test_jetson_to_v5", (SerialController::PacketCallback)SerialTestJetsonToV5Callback);
                this->serialController->ScheduleUpdate("serial_test_v5_to_jetson", 0.5);
            }
            else if(this->pUseSerial)  {        
                this->serialController->RegisterCallback("set_disk_obj", (SerialController::PacketCallback)SetDiskObjectCallback);
                this->serialController->RegisterCallback("get_disk_obj", (SerialController::PacketCallback)GetDiskObjectCallback);
                this->serialController->RegisterCallback("set_goal_obj", (SerialController::PacketCallback)SetGoalObjectCallback);
                this->serialController->RegisterCallback("get_goal_obj", (SerialController::PacketCallback)GetGoalObjectCallback);
                this->serialController->RegisterCallback("set_roller_obj", (SerialController::PacketCallback)SetRollerObjectCallback);
                this->serialController->RegisterCallback("get_roller_obj", (SerialController::PacketCallback)GetRollerObjectCallback);
                this->serialController->RegisterCallback("get_pos", (SerialController::PacketCallback)GetPositionCallback);
                this->serialController->RegisterCallback("set_pos", (SerialController::PacketCallback)SetPositionCallback);
                this->serialController->RegisterCallback("override_pos", (SerialController::PacketCallback)OverridePositionCallback);
                this->serialController->RegisterCallback("update_mag_count", (SerialController::PacketCallback)UpdateMagCountCallback);
                this->serialController->RegisterCallback("go_to_pos", (SerialController::PacketCallback)GoToPositionCallback);
                this->serialController->RegisterCallback("go_to_pos_dx_dy", (SerialController::PacketCallback)GoToPositionDxDyCallback);
                this->serialController->RegisterCallback("go_to_pos_dr_dtheta", (SerialController::PacketCallback)GoToPositionDRDThetaCallback);
                this->serialController->RegisterCallback("create_obj", (SerialController::PacketCallback)CreateObjectCallback);
                this->serialController->RegisterCallback("set_target_obj", (SerialController::PacketCallback)SetTargetObjectCallback);
                this->serialController->RegisterCallback("update_target_obj", (SerialController::PacketCallback)UpdateTargetObjectCallback);
                this->serialController->RegisterCallback("update_target_obj_dr_dtheta", (SerialController::PacketCallback)UpdateTargetObjectDRDThetaCallback);
                this->serialController->RegisterCallback("go_to_obj", (SerialController::PacketCallback)GoToObjectCallback);
                this->serialController->RegisterCallback("turn_to_angle", (SerialController::PacketCallback)TurnToAngleCallback);
                this->serialController->RegisterCallback("spin_roller", (SerialController::PacketCallback)SpinRollerCallback);
                this->serialController->RegisterCallback("turnoff_roller", (SerialController::PacketCallback)TurnOffRollerCallback);
                this->serialController->RegisterCallback("shoot_disk", (SerialController::PacketCallback)ShootDiskCallback);
                this->serialController->RegisterCallback("load_shooter", (SerialController::PacketCallback)LoadShooterCallback);
                this->serialController->RegisterCallback("clear_tasks", (SerialController::PacketCallback)ClearTasksCallback);
                this->serialController->RegisterCallback("task_finished", (SerialController::PacketCallback)TaskFinishedCallback);
                this->serialController->RegisterCallback("null_callback", (SerialController::PacketCallback)NullCallback);
                this->serialController->ScheduleUpdate("get_pos", 0.2);
            }
        }        
        this->serialController->ExchangeTags();
        if (! this->pDebug) 
        {/* Lets add in all of the data scheduling */
            this->serialController->ScheduleUpdate("update_mag_count", 1.0);
            this->serialController->ScheduleUpdate("get_pos", 0.1);
        }
        //pros::lcd::set_text(0,"Loading in System");
        
        // /* Init Object Handler */
        this->objHandler = new ObjectHandler(); /* TODO: ObjHandler Class Needs serious Update */
        this->objHandler->addObject(
            new Disk(Vector2::cartesianToVector2(0, 0), 0)
        );
        this->objHandler->addObject(
            new FieldRoller(Vector2::cartesianToVector2(0, 0), 0)
        );
        this->objHandler->addObject(
            new Goal(Vector2::cartesianToVector2(0, 0), 0, Goal::Color::RED) /* TODO: Whatever our current type is */
        );
        /* TASKS SECTION */
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::IntakeTask(-1, IntakeTask::ON, this->getSystem())}));

        this->taskManager->addTaskSet(new TaskList({new TerriBull::ShooterTask(ShooterTask::LOAD, nullptr, false, this->getSystem())}));
        this->taskManager->addTaskSet(new TaskList({new TerriBull::ShooterTask(ShooterTask::LOAD, nullptr, false, this->getSystem())}));
        this->taskManager->addTaskSet(new TaskList({new TerriBull::ShooterTask(ShooterTask::SHOOT, nullptr, false, this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::VariableTask(this->system->getDrive()->getRefMaxSpeed(), new float(70), VariableTask::FLOAT, this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(1000, this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::IntakeTask(1, IntakeTask::ON, this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::RollerTask(1000,this->getSystem())}));
        // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));
        // this->taskManager->addTaskSet(
        //     new TaskList({{
        //         TerriBull::DriveTask::DynamicInitialize(Vector2::cartesianToVector2(0, -17.0), true, TerriBull::DriveTask::TRANSLATION, this->getSystem()),
        //         // new TerriBull::RollerTask(0.75, 1, this->getSystem())
        //     }})
        // );

        // // this->taskManager->addTaskSet(new TaskList({new TerriBull::TimeTask(0.5, this->getSystem())}));

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
        //pros::lcd::set_text(0,"Parsing Error: "+::std::to_string(configParser->getErrCode()) );
        logger.logError(("Configuration Parsing Failed on initalization, Error Code: "+ ::std::to_string(configParser->getErrCode())));
        exit(1);
    }
}

void TerriBull::RoboController::Run() {
    this->updateTime();
    pros::lcd::set_text(7, "Framerate: "+ std::to_string(float(1.0 / this->delta())));
    this->system->Update(this->delta());
    this->serialController->Update(this->delta());
    if (pros::competition::is_autonomous()) { /*TODO: Or engaged Autonomous Control */
        if (this->taskManager->run(this->delta()) > 0 || this->taskManager->Interrupted()) {
            TaskFinishedCallback(this, nullptr, this->taskManager->GetTasksCompleted(), 0);
            this->taskManager->LowerInterruptFlag();
        }
    } else {
    /**
     * @DEBUG: THIS CODE SECTION
     */
        // Magazine* mag = static_cast<FlyWheelSB*>(this->system->getShooter())->getMag();
        // mag->update(this->delta());
    /**
     * @DEBUG: END
     */
        if(!this->pDebug) this->inputController->Update(this->delta());
        else {
            float voltages[] = {0, 0, 0, 0, 0, 0};
            this->system->getDrive()->setVoltage(voltages);
            if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
                this->taskManager->run(this->delta());
            }
        }
    }

    pros::delay(20);
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
    robot->getObjHandler()->Update(GameObject::ROLLER, id, update_args);
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
    bool isInContact = (bool) (int) SerialController::DeserializeNumber(array, &start_index);
    /*Update Sys*/
    void* update_args = FieldRoller::ConstructUpdateArgs(x, y, color, isInContact);
    robot->getObjHandler()->Update(GameObject::ROLLER, id, update_args);
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
 * DATA: [int id, float x, float y, d_theta, bool valid]
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
    bool valid = (bool) (int) SerialController::DeserializeNumber(array, &start_index);
    /*Update Sys*/
    void* update_args = Goal::ConstructUpdateArgs(x, y, dTheta, valid);
    robot->getObjHandler()->Update(GameObject::ROLLER, id, update_args);
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
 * DATA: => [float x, float y, float theta]
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
/**
 * @brief Override the Robot Internal Position Callback 
 * DATA: [float x, float y]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void OverridePositionCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float x = SerialController::DeserializeNumber(array, &start_index);
    float y = SerialController::DeserializeNumber(array, &start_index);
    Vector2* estimatePos = robot->getSystem()->getPosition();
    Vector2* interopPos = Vector2::cartesianToVector2(x, y);
    Vector2::UpdateValues(estimatePos, interopPos);
    delete interopPos;
}
/**
 * @brief Sends Mag count to the Jetson
 * DATA: =>[int mag_count]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void UpdateMagCountCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    std::stringstream s3;
    if ( (robot->getSystem()->getShooter()!= nullptr) && (robot->getSystem()->getShooter()->getType() == "FlyWheel-SB") ) {
        TerriBull::Magazine* mag = static_cast<FlyWheelSB*>(robot->getSystem()->getShooter())->getMag();
        s3 << (unsigned char) robot->getSerialController()->GetCallbackIndex("update_mag_count");
        s3 << SerialController::SerializeNumber(mag->getMagazineCount());
        robot->getSerialController()->SendData(s3.str());
    }
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
    Vector2* deltaPos = Vector2::polarToVector2(r, robot->getSystem()->getAngle()+ dtheta);
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        TerriBull::DriveTask::DynamicInitialize(deltaPos, reversed, TerriBull::DriveTask::ORIENTATION, robot->getSystem()),
    }));
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        TerriBull::DriveTask::DynamicInitialize(deltaPos, reversed, TerriBull::DriveTask::TRANSLATION, robot->getSystem()),
    }));
}
/**
 * @brief Create a Object Callback object
 * DATA: [int id, Union[GameObject::Type, int] type, float x, float y]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void CreateObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    char id = (char) (int) SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject::Types objectType = (TerriBull::GameObject::Types) (int) SerialController::DeserializeNumber(array, &start_index);
    float x = SerialController::DeserializeNumber(array, &start_index);
    float y = SerialController::DeserializeNumber(array, &start_index);
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
            obj = new Goal(pos, id, Goal::Color::RED); /*TODO: Opposite of starting Color.*/
            break;
        }
        default:
            break;
    }       
}

/**
 * @brief 
 * DATA: [int id, Union[GameObject::Type, int] type]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void SetTargetObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    char id = 0; //(char) (int) SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject::Types objectType = (TerriBull::GameObject::Types) (int) SerialController::DeserializeNumber(array, &start_index);
    GameObject* obj = robot->getObjHandler()->query(objectType, id);
    if (obj!= nullptr) {
        /*Update Sys*/
        robot->getSystem()->setTargetObject(obj);
        if (obj->type == GameObject::Types::GOAL) {
            Goal* goal = dynamic_cast<Goal*>(obj);
            goal->setValid(false);
        }
    }
}

/**
 * @brief Set the Target Object Callback
 * DATA: [float x, float y, char* params ] <- Will be determined based on the target object type
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void UpdateTargetObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float x = SerialController::DeserializeNumber(array, &start_index);
    float y = SerialController::DeserializeNumber(array, &start_index);
    GameObject* obj = robot->getSystem()->getTargetObject();
    if (obj != nullptr) {
        switch (obj->type) {
            case TerriBull::GameObject::Types::GOAL: { /* DATA: [..., float dTheta, bool valid, ] */
             /*Update Sys*/
             float dTheta = SerialController::DeserializeNumber(array, &start_index);
             bool valid = (bool) (int) SerialController::DeserializeNumber(array, &start_index);
             void* update_args = Goal::ConstructUpdateArgs(x, y, dTheta, valid);
             robot->getObjHandler()->Update(GameObject::GOAL, obj->id, update_args);
             break;   
            }
            case GameObject::Types::DISK: { /* DATA: [...] */
             /*Update Sys*/
             void* update_args = Disk::ConstructUpdateArgs(x, y);
             robot->getObjHandler()->Update(GameObject::DISK, obj->id, update_args);
             break;
            }
            case GameObject::Types::ROLLER: { /* DATA: [ ..., int color, bool isInContact] */
             /*Update Sys*/
             int color = (int) SerialController::DeserializeNumber(array, &start_index);
             bool isInContact = (bool) (int) SerialController::DeserializeNumber(array, &start_index);
             void* update_args = FieldRoller::ConstructUpdateArgs(x, y, color, isInContact);
             robot->getObjHandler()->Update(GameObject::ROLLER, obj->id, update_args);
             break;
            }
        }
    }
}

/**
 * @brief 
 * DATA: [float r, float d_theta, char* params ] <- Will be determined based on the target object type
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void UpdateTargetObjectDRDThetaCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float r = SerialController::DeserializeNumber(array, &start_index);
    float dtheta = SerialController::DeserializeNumber(array, &start_index);
    bool reversed = (bool) SerialController::DeserializeNumber(array, &start_index);
    Vector2* deltaPos = Vector2::polarToVector2(r, robot->getSystem()->getAngle()+ dtheta);
    Vector2* pos = *(robot->getSystem()->getPosition()) + *deltaPos;
    GameObject* obj = robot->getSystem()->getTargetObject();
    float x = pos->x;
    float y = pos->y;
    if (obj != nullptr) {
        GameObject* obj = robot->getSystem()->getTargetObject();
        if (obj != nullptr) {
            switch (obj->type) {
                case GameObject::Types::GOAL: { /* DATA: [..., float dTheta, bool valid, ] */
                 /*Update Sys*/
                 bool valid = (bool) (int) SerialController::DeserializeNumber(array, &start_index);
                 void* update_args = Goal::ConstructUpdateArgs(x, y, dtheta, valid);
                 robot->getObjHandler()->Update(GameObject::GOAL, obj->id, update_args);
                 delete static_cast<Goal::UpdateArgs*>(update_args);
                 break;   
                }
                case GameObject::Types::DISK: { /* DATA: [...] */
                 /*Update Sys*/
                 void* update_args = Disk::ConstructUpdateArgs(x, y);
                 robot->getObjHandler()->Update(GameObject::DISK, obj->id, update_args);
                 delete static_cast<Disk::UpdateArgs*>(update_args);
                 break;
                }
                case GameObject::Types::ROLLER: { /* DATA: [ ..., int color, bool isInContact] */
                 /*Update Sys*/
                 int color = (int) SerialController::DeserializeNumber(array, &start_index);
                 bool isInContact = (bool) (int) SerialController::DeserializeNumber(array, &start_index);
                 void* update_args = FieldRoller::ConstructUpdateArgs(x, y, color, isInContact);
                 robot->getObjHandler()->Update(GameObject::ROLLER, obj->id, update_args);
                 delete static_cast<FieldRoller::UpdateArgs*>(update_args);
                 break;
                }
            }
        }
    }
    delete deltaPos;
    delete pos;
}

/**
 * @brief Goes to an Object based on a identifier and type.
 * DATA: [ bool reversed ]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 * NOTE:  Note: IT IS NOT RECOMMENDED TO GO TO OBJECTS IN THE REVERSE DIRECTION
 * TODO: Needs ERRNO-like Message Passing  
 */
void GoToObjectCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    /* TODO: Add Features Such as this */
    // char id = (char) (int) SerialController::DeserializeNumber(array, &start_index);
    // TerriBull::GameObject::Types objectType = (TerriBull::GameObject::Types) (int) SerialController::DeserializeNumber(array, &start_index);
    bool reversed = (bool) (int) SerialController::DeserializeNumber(array, &start_index);
    // TerriBull::GameObject* obj = robot->getObjHandler()->query(objectType, id);    
    TerriBull::GameObject* obj = robot->getSystem()->getTargetObject();
    if (obj!= nullptr) {
        robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
            TerriBull::DriveTask::GoToObject(obj, reversed, robot->getSystem()),
        }));
    }
}
/**
 * @brief 
 * DATA: [float dtheta, bool reversed] <- we could implement as delta theta (we are)
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void TurnToAngleCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float dtheta = (int) SerialController::DeserializeNumber(array, &start_index);
    bool reversed = (bool) SerialController::DeserializeNumber(array, &start_index);
    float angle = (float) fmod(robot->getSystem()->getAngle() + dtheta, 360.0);
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::DriveTask(nullptr, angle, reversed, DriveTask::DriveType::ORIENTATION, robot->getSystem()),
    }));
}
/**
 * @brief 
 * DATA: [float pct0fMax]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void SpinRollerCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    float pctOfMax = SerialController::DeserializeNumber(array, &start_index);
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::RollerTask(RollerTask::RollerType::ON, robot->getSystem()),
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
void TurnOffRollerCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::RollerTask(RollerTask::RollerType::OFF, robot->getSystem()),
    }));
}
/**
 * @brief 
 * DATA: [char GOAL_ID]
 * @param robot 
 * @param array 
 * @param start_index 
 * @param length 
 */
void ShootDiskCallback(TerriBull::RoboController* robot, char * array, int start_index, int length) {
    char goal_id = 0; // SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject* obj = robot->getObjHandler()->query(GameObject::Types::GOAL, 0); 
    /*TODO: parse for extra args for update */
    if (obj == nullptr) { return; } /* TODO: Needs ERRNO-like Message Passing */
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::ShooterTask(ShooterTask::SHOOT, nullptr, false, robot->getSystem()),
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
    int disk_id = 0;// SerialController::DeserializeNumber(array, &start_index);
    TerriBull::GameObject* obj = robot->getObjHandler()->query(GameObject::Types::DISK, disk_id);    
    if (obj == nullptr) { return; } /* TODO: Needs ERRNO-like Message Passing */
    /*TODO: parse for extra args for update */
    robot->getTaskManager()->addTaskSet(new TerriBull::TaskList({
        new TerriBull::ShooterTask(ShooterTask::SHOOT, nullptr, false, robot->getSystem()),
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
/**
 * @brief Returns the number of tasks Completed once a task is completed.
 * DATA: =>[int num_tasks_completed]
 * @param robot 
 * @param array 
 * @param start 
 * @param length 
 */
void TaskFinishedCallback(TerriBull::RoboController* robot, char * array, int start, int length) {
    std::stringstream s3;
    s3 << (unsigned char) robot->getSerialController()->GetCallbackIndex("task_finished");
    s3 << SerialController::SerializeNumber(start); /*start is the num_tasks :3 */
    robot->getSerialController()->SendData(s3.str());
}

/* Other Callbacks */
void TagExchangeCallback(TerriBull::RoboController* robot, char * array, int start, int length) {
    // robot->getSerialController()->updateExchangeTags();
}
/**
 * @brief Sends Test Data to the Jetson
 * 
 * @param robot 
 * @param array 
 * @param start 
 * @param length 
 */
void SerialTestV5ToJetsonCallback(TerriBull::RoboController* robot, char * array, int start, int length) {
    std::stringstream s3;
    float theta = robot->getSystem()->getAngle();
    s3 << (unsigned char) robot->getSerialController()->GetCallbackIndex("serial_test_v5_to_jetson");
    s3 << SerialController::SerializeNumber(theta);
    s3 << SerialController::SerializeString("Digits of Pi:");
    s3 << SerialController::SerializeNumber(3.14159);
    robot->getSerialController()->SendData(s3.str());
}
/**
 * @brief Recieves Test Data from the Jetson
 * 
 * @param robot 
 * @param array 
 * @param start 
 * @param length 
 */
void SerialTestJetsonToV5Callback(TerriBull::RoboController* robot, char * array, int start, int length) {
    double digitsOfPi = SerialController::DeserializeNumber(array, &start);
    std::string s1 = SerialController::DeserializeString(array, &start);
    string dateTime = SerialController::DeserializeString(array, &start);
    pros::lcd::set_text(2, s1);
    pros::lcd::set_text(4, to_string(digitsOfPi));
    pros::lcd::set_text(5, dateTime);
}

void NullCallback(TerriBull::RoboController* robot, char * array, int start, int length) {
    /* Does Nothing */
}