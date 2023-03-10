/**
 * @file TimeTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot For Time Delay.
 *     
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TIMETASK_H
#define TIMETASK_H

// #include "../Task.hpp"
#include "../../../TerriBull.hpp"

class TerriBull::TimeTask : public TerriBull::Task {
    public: 
    // typedef enum {ON, } RollerType;
    private:
    float sumTime;
    float goalTime;
    // RollerType rollerType;    
    public:
    
    TimeTask(float goalTime, TerriBull::MechanicalSystem* _system);


    ~TimeTask();

    void init();
    void update(float delta);
    void terminate();  
};


#endif  /* SHOOTERTASK_H */