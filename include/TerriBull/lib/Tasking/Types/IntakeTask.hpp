/**
 * @file IntakeTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the Intake system of the BullBot.
 *     
 * @version 0.1
 * @date 2023-03-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef INTAKETASK_H
#define INTAKETASK_H

// #include "../Task.hpp"
#include "../../../TerriBull.hpp"

class TerriBull::IntakeTask : public TerriBull::Task {
    public: 
    typedef enum {ON, OFF} IntakeType;
    
    private:
    float dir;
    IntakeType intakeType;
    TerriBull::MechanicalSystem* system;

    public:
    
    IntakeTask(float dir, IntakeType intakeType, TerriBull::MechanicalSystem* _system);
    ~IntakeTask() {}

    void init();
    void update(float delta);  
    void terminate();
};


#endif  /* INTAKETASK_H */