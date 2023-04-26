
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

RollerTask* RollerTask::TurnOn(float pct0fMax, TerriBull::MechanicalSystem* _system) {
    RollerTask* task = new RollerTask(_system);
    task->needsInitialize = false;
    task->deleteOnCleanup = false;
    task->needsInitialize = false;
    task->rollerType = ON;
    task->pct0fMax = pct0fMax;
    return task;
} 
RollerTask* RollerTask::TurnOff(TerriBull::MechanicalSystem* _system) {
    RollerTask* task = new RollerTask(_system);
    task->needsInitialize = false;
    task->deleteOnCleanup = false;
    task->needsInitialize = false;
    task->rollerType = OFF;
    return task;
}

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
    this->system->ResetRoller();
}

void RollerTask::update(float delta) {
    // pros::lcd::set_text(7, "Roller Tasks...");
    if (!this->finishedFlag) {
        switch(rollerType) {
            case TIME:
                this->system->SpinRollerFor(this->direction, this->time);
                this->finishedFlag = this->system->isRollerCompleted();
                if (this->finishedFlag) this->system->TurnOffRoller();
                break;
            case POS:
                this->system->SpinRollerTo(this->targetPos);
                this->finishedFlag = fabs(this->system->getRollerError()) < 2 && fabs(this->system->getRollerDError()) < 0.5;
                if (this->finishedFlag) this->system->TurnOffRoller();
                break;
            case ON:
                this->system->TurnOnRoller(this->pct0fMax);
                this->finishedFlag = this->system->isRollerToggled();
            case OFF:
                this->system->TurnOffRoller();
                this->finishedFlag =  !( this->system->isRollerToggled() );
        }
    } if(this->finishedFlag) ;
}

void RollerTask::terminate() {
    this->terminated = true;
    this->system->ResetRoller();
}