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
DriveTask::DriveTask(TerriBull::Vector2* pos, float _orientation, bool reversed, DriveType _driveType, TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), targetTheta(_orientation), driveType(_driveType), calculateOnInit(false), reversed(reversed), needsInitialize(false) {
    this->v_f = pos;
    this->deleteOnCleanup = false;
    this->targetTheta = _orientation;
    this->deleteOnCleanup = true;
    if (_orientation == 420) {
        calculateOnInit = true;
    }
}

/* Make Orientation 420 to Have it auto Calculate based on the position provided. */
DriveTask::DriveTask(TerriBull::Vector2 pos, float _orientation, bool reversed, DriveType _driveType, TerriBull::MechanicalSystem* _system) : Task(DRIVE, _system), targetTheta(_orientation), driveType(_driveType), calculateOnInit(false), reversed(reversed), needsInitialize(false) {
    this->v_f = new TerriBull::Vector2(pos);
    this->deleteOnCleanup = true;
    if (_orientation == 420) {
        calculateOnInit = true;
    }
}

DriveTask::~DriveTask() {
    if (this->deleteOnCleanup) {
        delete this->v_f;
        delete this->v_i;
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

DriveTask* DriveTask::GoToObject(TerriBull::GameObject* object, bool reversed,TerriBull::MechanicalSystem* system) {
    return nullptr;
}

void DriveTask::init() {
    this->finishedFlag = false;
    this->hitTarget = false;
    this->system->resetDrive();
    this->v_i = new TerriBull::Vector2(this->system->getPosition());
    if (this->calculateOnInit) {
        this->v_f = *(v_i) + *this->offset;
        float angleMod = (this->reversed) ? 180 : 0;
        Vector2* dPos = *(this->v_f) - *(v_i);
        this->targetTheta = fmod(RAD2DEG(dPos->theta) + angleMod, 360);
        delete dPos;
    }
    if (this->needsInitialize) {
        this->v_f = *(v_i) + *this->offset;
        Vector2* dPos = *(this->v_f) - *(v_i);
        this->deleteOnCleanup = true;
        float angleMod = (this->reversed) ? 180 : 0;
        this->targetTheta = fmod(RAD2DEG(dPos->theta) + angleMod, 360);
        switch(this->driveType) {
            case TRANSLATION:
                break;
            case ORIENTATION:
                break;
        } delete dPos;
    }
}

void DriveTask::update(float delta) {
    if (!this->finishedFlag) {
        switch(driveType) {
            case TRANSLATION:
                if (!this->hitTarget) {
                    // bool currentCorrectionStatus = this->system->driveNeedsAngleCorrection();
                    // if (currentCorrectionStatus) {
                    //     if (this->lastNeedsCorrection != this->system->driveNeedsAngleCorrection()) {
                    //          this->system->resetDrive();
                    //     }
                    //     this->lastNeedsCorrection = currentCorrectionStatus;
                    //     Vector2* dPos = *(this->v_f) - *(this->system->getPosition());
                    //     float angleMod = (this->reversed) ? 180 : 0;
                    //     float angleCorrection = fmod(RAD2DEG(dPos->theta) + angleMod, 360);
                    //     this->system->TurnToAngle(angleCorrection);
                    //     this->system->getDrive()->updateAngleCorrection(fabs(this->system->getDriveError()) < 1 && (fabs(this->system->getDriveDError()) / delta) < 0.1);
                    //     delete dPos;
                    // }
                    // else {
                        this->system->GoToPosition(*(this->v_f), *(this->v_i), this->reversed); /*TODO: Test Delta Value  */
                        this->hitTarget = fabs(this->system->getDriveError()) < 1.6 && (fabs(this->system->getDriveDError()) / delta) < 0.25; 
                }
            //    }
                else {
                    this->system->TurnToAngle(this->targetTheta);
                    this->finishedFlag = fabs(this->system->getDriveError()) < 0.38 && (fabs(this->system->getDriveDError()) / delta) < 0.01; 
               } 
            break;
            case ORIENTATION:
                this->system->TurnToAngle(this->targetTheta);
                this->finishedFlag = fabs(this->system->getDriveError()) < 0.38 && (fabs(this->system->getDriveDError()) / delta) < 0.01; 
                break;
        }
        
    }
}

void DriveTask::terminate() {
    this->terminated = true;
    this->system->resetDrive();
}