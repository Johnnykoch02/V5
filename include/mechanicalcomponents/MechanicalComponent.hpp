/**
 * @file MechanicalSystem.hpp
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
    TerriBull::string pType;
    TerriBull::Str2SizeMap pPorts;
    int pVoltageCap = TerriBull::MAX_VOLTAGE;
    //Vector for
    float currentError;
    float previousError;

    float kP, kD, kI;

    public:
    MechanicalComponent();
    MechanicalComponent(const string type, const TerriBull::Str2SizeMap  ports) :
     pType(type), pPorts(ports), kP(0), kD(0), kI(0), currentError(0), previousError(0) {}
    virtual float dError() const final {
        return (currentError - previousError);
    }
    float getError() const final {
        return currentError;
    } 
    float dError(){
        return (this->currentError - this->previousError);
    }

    virtual ::std::string getType() const final {
        return this->pType;
    };
};


#endif