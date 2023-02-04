/**
 * @file DriveTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot.
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */#ifndef DRIVETASK_H
#define DRIVETASK_H

// #include "../Task.hpp"
#include "../../../TerriBull.hpp"

class DriveTask : public TerriBull::Task {
    private:
    float approachOrientation;
    TerriBull::Vector2* pos;
    typedef enum {TRANSLATION, ORIENTATION} DriveType;
    DriveType driveType;
    TerriBull::MechanicalSystem* system;

    bool deleteOnCleanup;

    public:

    DriveType taskType;
    DriveTask(TerriBull::Vector2* pos, float _orientation, DriveType _driveType, TerriBull::MechanicalSystem* _system) : approachOrientation(_orientation), driveType(_driveType) {
        this->pos = pos;
        this->deleteOnCleanup = false;
        this->approachOrientation = _orientation;
        this->system = _system;
    }

    DriveTask(TerriBull::Vector2 pos, float _orientation, DriveType _driveType, TerriBull::MechanicalSystem* _system) : approachOrientation(_orientation), driveType(_driveType) {
        this->pos = new Vector2(pos);
        this->deleteOnCleanup = true;
        this->approachOrientation = _orientation;
        this->system = _system;
    }

    ~DriveTask() {
        if (this->deleteOnCleanup) {
            delete this->pos;
        }
    }

    void init();
    void update(float delta);  
};

void DriveTask::init() {
    this->finishedFlag = false;
}

void DriveTask::update(float delta) {
    if (!this->finishedFlag) {
        switch(driveType) {
            case TRANSLATION:
                this->system->GoToPosition(this->pos->x, this->pos->y);
                this->finishedFlag = this->system->getDriveError() < 0.05; /* Some Threshold */
                break;
            case ORIENTATION:
                this->system->TurnToAngle(this->approachOrientation);
                this->finishedFlag = this->system->getDriveError() < 0.05; /* Some Threshold */
                break;
        }
    }
}


#endif  /* DRIVETASK_H */