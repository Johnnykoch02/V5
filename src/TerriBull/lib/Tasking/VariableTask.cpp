/**
 * @file VariableTask.vpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot For modifying program variables.
 *     
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../../../include/TerriBull/lib/Tasking/Types/VariableTask.hpp"
VariableTask::VariableTask(void* targetVariable, void* targetValue, VarType _varType, TerriBull::MechanicalSystem* _system) : Task(VARIABLE, _system), varType(_varType) {
    this->targetVariable = targetVariable;
    this->targetValue = targetValue;
}

VariableTask::~VariableTask() {}

void VariableTask::init() {
    this->finishedFlag = false;
}

void VariableTask::update(float delta) {
    if (!this->finishedFlag) {
        switch (varType) {
            case INT:
                *(int*)targetVariable = *(int*)targetValue;
                break;
            case FLOAT:
                *(float*)targetVariable = *(float*)targetValue;
                break;
            case CHAR:
                *(char*)targetVariable = *(char*)targetValue;
                break;
            case DOUBLE:
                *(double*)targetVariable = *(double*)targetValue;
                break;
            case BOOL: 
                *(bool*)targetVariable = *(bool*)targetValue;
                break;
            default:
                break;
        }
        this->finishedFlag = true;
    }
}

void VariableTask::terminate() {
    if (!this->finishedFlag) {
        switch (varType) {
            case INT:
                delete (int*)targetValue;
                break;
            case FLOAT:
                delete (float*)targetValue;
                break;
            case CHAR:
                delete (char*)targetValue;
                break;
            case DOUBLE:
                delete (double*)targetValue;
                break;
            case BOOL: 
                delete (bool*)targetValue;
                break;
            default:
                break;
        }
    }
    this->terminated = true;
}