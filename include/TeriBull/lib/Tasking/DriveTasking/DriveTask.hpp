#ifndef DRIVETASK_H
#define DRIVETASK_H

#include "../Task.hpp"

class DriveTask : public TerriBull::Task {
    private:
    float x, y, approachOrientation;

    virtual void init() = 0;
    virtual void update(float delta) = 0;
    
}

#endif  /* DRIVETASK_H */