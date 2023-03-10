/**
 * @file ShooterTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot For Shooting Disks.
 *     
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SHOOTERTASK_H
#define SHOOTERTASK_H

// #include "../Task.hpp"
#include "../../../TerriBull.hpp"

class TerriBull::ShooterTask : public TerriBull::Task {
    public: 
    // typedef enum {ON, } RollerType;
    private:
    // RollerType rollerType;
    
    public:
    
    ShooterTask(TerriBull::MechanicalSystem* _system);


    ~ShooterTask();

    void init();
    void update(float delta);
    void terminate();  
};


#endif  /* SHOOTERTASK_H */