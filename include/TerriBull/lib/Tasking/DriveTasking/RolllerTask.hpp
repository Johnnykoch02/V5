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
#ifndef ROLLERTASK_H
#define ROLLERTASK_H

// #include "../Task.hpp"
#include "../../../TerriBull.hpp"

class TerriBull::RollerTask : public TerriBull::Task {
    public: 
    
    private:
    float approachOrientation;
    TerriBull::Vector2* pos;

    

    bool deleteOnCleanup;

    public:
    
    RollerTask(TerriBull::Vector2* pos, float _orientation, TerriBull::MechanicalSystem* _system);

    RollerTask(TerriBull::Vector2 pos, float _orientation, TerriBull::MechanicalSystem* _system);

    ~RollerTask();

    void init();
    void update(float delta);  
};


#endif  /* DRIVETASK_H */