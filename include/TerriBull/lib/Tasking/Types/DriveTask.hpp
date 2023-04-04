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
    typedef enum {TRANSLATION, ORIENTATION, OBJECT} DriveType;
    
    private:
    float approachOrientation;
    TerriBull::Vector2* pos;
    DriveType driveType;

    TerriBull::Vector2* offset;
    bool calculateOnInit;
    bool needsInitialize;
    bool deleteOnCleanup;
    bool reversed;

    DriveTask(TerriBull::MechanicalSystem* _system);
    public:
    
    DriveTask(TerriBull::Vector2* pos, float _orientation, bool reversed, DriveType _driveType, TerriBull::MechanicalSystem* _system);
    DriveTask(TerriBull::Vector2 pos, float _orientation, bool reversed, DriveType _driveType, TerriBull::MechanicalSystem* _system);
    
    static DriveTask* GoToObject(TerriBull::GameObject* object, bool reversed,TerriBull::MechanicalSystem* system);
    static DriveTask* DynamicInitialize(Vector2* offset, bool reversed, DriveType driveType, TerriBull::MechanicalSystem* system);

    ~DriveTask();

    void init();
    void update(float delta);  
    void terminate();
};


#endif  /* DRIVETASK_H */