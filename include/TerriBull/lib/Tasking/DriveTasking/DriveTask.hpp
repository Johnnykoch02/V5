#ifndef DRIVETASK_H
#define DRIVETASK_H

#include "../Task.hpp"
#include "../../../TerriBull.hpp"

class DriveTask : public TerriBull::Task {
    private:
    float approachOrientation;
    Vector2* pos;
    typedef enum {TRANSLATION, ORIENTATION} DriveType;
    DriveType driveType;
    MechanicalSystem* system;

    bool deleteOnCleanup;

    public:

    DriveType taskType;
    DriveTask(Vector2* pos, float _orientation, DriveType _driveType, MechanicalSystem* _system) approachOrientation(_orientation), driveType(_driveType) {
        this->pos = pos;
        this->deleteOnCleanup = false;
        this->orientation = _orientation;
        this->system = _system;
    }

    DriveTask(Vector2 pos, float _orientation, DriveType _driveType, MechanicalSystem* _system) approachOrientation(_orientation), driveType(_driveType) {
        this->pos = new Vector2(pos);
        this->deleteOnCleanup = true;
        this->orientation = _orientation;
        this->system = _system;
    }

    ~DriveTask() {
        if (this->deleteOnCleanup) {
            delete this->pos;
        }
    }

    void init();
    void update(float delta);  
}

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
                this->system->TurnToAngle(this->orientation);
                this->finishedFlag = this->system->getDriveError() < 0.05; /* Some Threshold */
                break;
        }
    }
}


#endif  /* DRIVETASK_H */