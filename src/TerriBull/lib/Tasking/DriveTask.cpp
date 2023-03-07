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
#include "../../../../include/TerriBull/lib/Tasking/DriveTasking/DriveTask.hpp"
DriveTask::DriveTask(TerriBull::Vector2* pos, float _orientation, DriveType _driveType, TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), approachOrientation(_orientation){
    this->pos = pos;
    this->deleteOnCleanup = false;
    this->approachOrientation = _orientation;
    this->system = _system;
}

DriveTask::DriveTask(TerriBull::Vector2 pos, float _orientation, DriveType _driveType, TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), approachOrientation(_orientation){
    this->pos = new TerriBull::Vector2(pos);
    this->deleteOnCleanup = true;
    this->approachOrientation = _orientation;
    this->system = _system;
}

DriveTask::~DriveTask() {
    if (this->deleteOnCleanup) {
        delete this->pos;
    }
}

void DriveTask::init() {
    this->finishedFlag = false;
}

void DriveTask::update(float delta) {
    if (!this->finishedFlag) {
        switch(driveType) {
            case TRANSLATION:
                this->system->GoToPosition(this->pos->x, this->pos->y);
                break;
            case ORIENTATION:
                this->system->TurnToAngle(this->approachOrientation);
                break;
        }
        this->finishedFlag = fabs(this->system->getDriveError()) < 0.5 && fabs(this->system->getDriveDError()) < 0.25; /* Some Threshold */
    }
}
