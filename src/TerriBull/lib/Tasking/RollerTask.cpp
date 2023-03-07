
 /**
 * @file RollerTask.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot.
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../../../include/TerriBull/lib/Tasking/Types/RollerTask.hpp"
RollerTask::RollerTask(TerriBull::MechanicalSystem* _system) : Task(ROLLER, _system) {}

RollerTask::RollerTask(float time, int direction, TerriBull::MechanicalSystem* _system) : Task(ROLLER, _system), needsInitialize(false), time(time), direction(direction), rollerType(TIME) {}

RollerTask::RollerTask(float pos, TerriBull::MechanicalSystem* _system) : Task(ROLLER, _system), needsInitialize(false), targetPos(pos), rollerType(POS) {}

RollerTask* RollerTask::DynamicInitialize(float* pos, float offset, TerriBull::MechanicalSystem* _system) {
    RollerTask* task = new RollerTask(_system);
    task->deleteOnCleanup = false;
    task->needsInitialize = true;
    task->initialPos = pos;
    task->offset = offset;
    task->targetPos = 0;
    task->rollerType = POS;
    return task;
}

RollerTask::~RollerTask() {}

void RollerTask::init() {
    this->finishedFlag = false;
    if(this->needsInitialize) {
        this->targetPos = *(this->initialPos) + this->offset;
    }
    this->system->resetRoller();
}

void RollerTask::update(float delta) {
    pros::lcd::set_text(7, "Roller Tasks...");
    if (!this->finishedFlag) {
        switch(rollerType) {
            case TIME:
                this->system->spinRollerFor(this->direction, this->time);
                this->finishedFlag = this->system->getRoller()->timeFlag;
                break;
            case POS:
                this->system->spinRollerTo(this->targetPos);
                this->finishedFlag = fabs(this->system->getRollerError()) < 0.2 && fabs(this->system->getRollerDError()) < 0.5;
                break;
        }
    } if(this->finishedFlag) pros::lcd::set_text(7, "Roller Task Finished.");
}

void RollerTask::terminate() {
    this->terminated = true;
    this->system->resetRoller();
}