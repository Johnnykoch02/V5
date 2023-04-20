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
    public:
    struct MotorRefs {
        ::std::string ComponentName;
        pros::Motor** Motors;
        size_t NumMotors;
    };
    protected:
    ::std::string pType;
    int pVoltageCap;
    //Vector for
    float currentError;
    float previousError;
    float sumError;
    pros::Motor** pMotorRefs;    
    int gearSet;
    bool pToggled;

    float kP, kD, kI;
    MotorRefs *motorRefs;
 
    public:
    MechanicalComponent(int gearSet) : kP(0), kD(0), kI(0), currentError(0), previousError(0), pVoltageCap(TerriBull::MAX_VOLTAGE), gearSet(gearSet), pToggled(false) {}
 
    virtual float getError() const final { return currentError; } 

    /**
     * @brief Gets Current Derror and automatically sets the previous error value to the current error value.
     * WARNING: this function is not READONLY
     * @return float 
     */
    virtual float dError() final {
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

    virtual bool isReset() const final { return this->getError() == 0; }

    virtual void setPID(float kP, float kI, float kD) final {
        this->kP = kP;
        this->kD = kD;
        this->kI = kI;
    }


    virtual ::std::string getType() const final {
        return this->pType;
    };

    virtual float getRPM() const {
        return 0;
    }
    virtual bool getToggled() const final {
        return pToggled;
    }

    virtual MotorRefs* getMotorRefs() const final {
        return this->motorRefs;
    }

};


#endif