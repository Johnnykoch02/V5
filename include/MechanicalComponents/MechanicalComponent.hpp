/**
 * @file MechanicalComponent.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Mechanical componenent base class for robot components
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MECHANICAL_COMPONENT
#define MECHANICAL_COMPONENT

#include "../TerriBull/TerriBull.hpp"

class TerriBull::MechanicalComponent  {
    protected:
    ::std::string pType;
    int pVoltageCap;
    //Vector for
    float currentError;
    float previousError;
    float sumError;

    float kP, kD, kI;

    public:
    MechanicalComponent() : kP(0), kD(0), kI(0), currentError(0), previousError(0), pVoltageCap(TerriBull::MAX_VOLTAGE) {}
 
    virtual float getError() const final {
        return currentError;
    } 
    virtual float dError() const final{
        return (this->currentError - this->previousError);
    }

    virtual void setPID(float kP, float kD, float kI) final {
        this->kP = kP;
        this->kD = kD;
        this->kI = kI;
    }

    virtual ::std::string getType() const final {
        return this->pType;
    };
};


#endif