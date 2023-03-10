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
/* Make Orientation 420 to Have it auto Calculate. */
DriveTask::DriveTask(TerriBull::Vector2* pos, float _orientation, bool reversed, DriveType _driveType, TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), approachOrientation(_orientation), driveType(_driveType), calculateOnInit(false), reversed(reversed) {
    this->pos = pos;
    this->deleteOnCleanup = false;
    this->approachOrientation = _orientation;
    this->deleteOnCleanup = true;
    if (_orientation == 420) {
        calculateOnInit = true;
    }
}

/* Make Orientation 420 to Have it auto Calculate based on the position provided. */
DriveTask::DriveTask(TerriBull::Vector2 pos, float _orientation, bool reversed, DriveType _driveType, TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), approachOrientation(_orientation), driveType(_driveType), calculateOnInit(false), reversed(reversed) {
    this->pos = new TerriBull::Vector2(pos);
    this->deleteOnCleanup = true;
    if (_orientation == 420) {
        calculateOnInit = true;
    }
}

DriveTask::~DriveTask() {
    if (this->deleteOnCleanup) {
        delete this->pos;
    }
}

void DriveTask::init() {
    this->finishedFlag = false;
    this->system->resetDrive();
    if (this->calculateOnInit) {
        float angleMod = (this->reversed) ? 180 : 0;
        Vector2* dPos = *(this->pos) - *(this->system->getPosition());
        this->approachOrientation = fmod(RAD2DEG(dPos->theta) + angleMod, 360);
        delete dPos;
    }
}

void DriveTask::update(float delta) {
    if (!this->finishedFlag) {
        switch(driveType) {
            case TRANSLATION:
                this->system->GoToPosition(*(this->pos));
                break;
            case ORIENTATION:
                this->system->TurnToAngle(this->approachOrientation);
                break;
        }
        this->finishedFlag = fabs(this->system->getDriveError()) < 0.8 && fabs(this->system->getDriveDError()) < 0.25; /* Some Threshold */
    }
}

void DriveTask::terminate() {
    this->terminated = true;
    this->system->resetDrive();
}