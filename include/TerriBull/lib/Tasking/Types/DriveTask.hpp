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
 */
#ifndef DRIVETASK_H
#define DRIVETASK_H

// #include "../Task.hpp"
#include "../../../TerriBull.hpp"

class TerriBull::DriveTask : public TerriBull::Task {
    public: 
    typedef enum {TRANSLATION, ORIENTATION} DriveType;
    
    private:
    float approachOrientation;
    TerriBull::Vector2* pos;
    DriveType driveType;

    bool deleteOnCleanup;

    public:
    
    DriveTask(TerriBull::Vector2* pos, float _orientation, DriveType _driveType, TerriBull::MechanicalSystem* _system);

    DriveTask(TerriBull::Vector2 pos, float _orientation, DriveType _driveType, TerriBull::MechanicalSystem* _system);

    ~DriveTask();

    void init();
    void update(float delta);  
    void terminate();
};


#endif  /* DRIVETASK_H */