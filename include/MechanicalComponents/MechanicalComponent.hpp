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

    int gearSet;

    float kP, kD, kI;

    public:
    MechanicalComponent(int gearSet) : kP(0), kD(0), kI(0), currentError(0), previousError(0), pVoltageCap(TerriBull::MAX_VOLTAGE), gearSet(gearSet) {}
 
    virtual float getError() const final { return currentError; } 

    /**
     * @brief Gets Current Derror and automatically sets the previous error value to the current error value.
     * WARNING: this function is not READONLY
     * @return float 
     */
    virtual float dError() final{
        float dError = currentError - previousError;
        this->previousError = currentError;
        return dError;
    }
    /**
     * @brief READONLY: Gets the current derror value.
     * 
     * @return float 
     */
    virtual float ROdError() const final{
        return currentError - previousError;
    }

    virtual bool isReset() const final { return this->sumError == 0; }

    virtual void setPID(float kP, float kI, float kD) final {
        this->kP = kP;
        this->kD = kD;
        this->kI = kI;
    }


    virtual ::std::string getType() const final {
        return this->pType;
    };
};


#endif