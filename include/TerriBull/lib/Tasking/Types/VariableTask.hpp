/**
 * @file VariableTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot For modifying program variables.
 *     
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef VARIABLETASK_H
#define VARIABLETASK_H

// #include "../Task.hpp"
#include "../../../TerriBull.hpp"

class TerriBull::VariableTask : public TerriBull::Task {
    public: 
    typedef enum { INT, FLOAT, DOUBLE, CHAR, BOOL } VarType;
    private:
    void* targetVariable;
    void* targetValue;
    VarType varType;    
    public:
    
    VariableTask(void* targetVariable, void* targetValue, VarType _varType, TerriBull::MechanicalSystem* _system);


    ~VariableTask();

    void init();
    void update(float delta);
    void terminate();  
};


#endif  /* SHOOTERTASK_H */