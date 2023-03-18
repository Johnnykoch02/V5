/**
 * @file DriveTask.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot.
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../../../include/TerriBull/lib/Tasking/Types/DriveTask.hpp"


DriveTask::DriveTask(TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), needsInitialize(false) {}

/* Make Orientation 420 to Have it auto Calculate. */
DriveTask::DriveTask(TerriBull::Vector2* pos, float _orientation, bool reversed, DriveType _driveType, TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), approachOrientation(_orientation), driveType(_driveType), calculateOnInit(false), reversed(reversed), needsInitialize(false) {
    this->pos = pos;
    this->deleteOnCleanup = false;
    this->approachOrientation = _orientation;
    this->deleteOnCleanup = true;
    if (_orientation == 420) {
        calculateOnInit = true;
    }
}

/* Make Orientation 420 to Have it auto Calculate based on the position provided. */
DriveTask::DriveTask(TerriBull::Vector2 pos, float _orientation, bool reversed, DriveType _driveType, TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), approachOrientation(_orientation), driveType(_driveType), calculateOnInit(false), reversed(reversed), needsInitialize(false) {
    this->pos = new TerriBull::Vector2(pos);
    this->deleteOnCleanup = true;
    if (_orientation == 420) {
        calculateOnInit = true;
    }
}

DriveTask::~DriveTask() {
    if (this->deleteOnCleanup) {
        delete this->pos;
        delete this->offset;
    }
}

DriveTask* DriveTask::DynamicInitialize(Vector2* offset, bool reversed, DriveType driveType, TerriBull::MechanicalSystem* system) {
    DriveTask* task = new DriveTask(system);
    task->reversed = reversed;
    task->deleteOnCleanup = false;
    task->needsInitialize = true;
    task->offset = offset;
    task->driveType = driveType;
    return task;
}

void DriveTask::init() {
    this->finishedFlag = false;
    this->system->resetDrive();
    if (this->calculateOnInit) {
        this->pos = *(this->system->getPosition()) + *this->offset;
        float angleMod = (this->reversed) ? 180 : 0;
        Vector2* dPos = *(this->pos) - *(this->system->getPosition());
        this->approachOrientation = fmod(RAD2DEG(dPos->theta) + angleMod, 360);
        delete dPos;
    }
    if (this->needsInitialize) {
        this->pos = *(this->system->getPosition()) + *this->offset;
        Vector2* dPos = *(this->pos) - *(this->system->getPosition());
        this->deleteOnCleanup = true;
        switch(this->driveType) {
            case TRANSLATION:
                break;
            case ORIENTATION:
                float angleMod = (this->reversed) ? 180 : 0;
                this->approachOrientation = fmod(RAD2DEG(dPos->theta) + angleMod, 360);
        } delete dPos;
    }
}

void DriveTask::update(float delta) {
    if (!this->finishedFlag) {
        switch(driveType) {
            case TRANSLATION:
                this->system->GoToPosition(*(this->pos)); /*TODO: Test Delta Value  */
                this->finishedFlag = fabs(this->system->getDriveError()) < 0.35 && (fabs(this->system->getDriveDError()) / delta) < 0.25; 
                break;
            case ORIENTATION:
                this->system->TurnToAngle(this->approachOrientation);
                this->finishedFlag = fabs(this->system->getDriveError()) < 0.3 && (fabs(this->system->getDriveDError()) / delta) < 0.01; 
                break;
        }
        
    }
}

void DriveTask::terminate() {
    this->terminated = true;
    this->system->resetDrive();
}