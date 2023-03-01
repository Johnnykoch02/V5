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
DriveTask::DriveTask(TerriBull::Vector2* pos, float _orientation, DriveType _driveType, TerriBull::MechanicalSystem* _system) : approachOrientation(_orientation), driveType(_driveType) {
    this->pos = pos;
    this->deleteOnCleanup = false;
    this->approachOrientation = _orientation;
    this->system = _system;
}

DriveTask::DriveTask(TerriBull::Vector2 pos, float _orientation, DriveType _driveType, TerriBull::MechanicalSystem* _system) : approachOrientation(_orientation), driveType(_driveType) {
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
                this->finishedFlag = this->system->getDriveError() < 0.05 && this->system->getDriveDError() < 0.05; /* Some Threshold */
                break;
            case ORIENTATION:
                this->system->TurnToAngle(this->approachOrientation);
                this->finishedFlag = this->system->getDriveError() < 0.05 && this->system->getDriveDError() < 0.05;; /* Some Threshold */
                break;
        }
    }
}
