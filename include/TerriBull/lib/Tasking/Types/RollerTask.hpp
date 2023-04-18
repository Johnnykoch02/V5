/**
 * @file RollerTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot.
 *     
 * @version 0.1
 * @date 2023-03-06
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
    typedef enum {POS, TIME, ON, OFF, OBJ} RollerType;
    private:
    float* initialPos;
    float offset;
    float targetPos;
    float time;
    float direction;
    bool deleteOnCleanup;
    bool needsInitialize;
    RollerType rollerType;

    RollerTask(TerriBull::MechanicalSystem* _system);
    
    public:
    
    RollerTask(float time, int direction, TerriBull::MechanicalSystem* _system);

    RollerTask(float pos, TerriBull::MechanicalSystem* _system);

    static RollerTask* DynamicInitialize(float* pos, float offset, TerriBull::MechanicalSystem* _system);
    static RollerTask* TurnOn(int dir, float pwr, TerriBull::MechanicalSystem* _system);
    static RollerTask* TurnOff(TerriBull::MechanicalSystem* _system);
    ~RollerTask();

    void init();
    void update(float delta);
    void terminate();  
};


#endif  /* ROLLERTASK_H */