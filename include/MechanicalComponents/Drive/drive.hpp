/**
 * @file MechanicalSystem.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief drive base class for driver system
 *
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DRIVE_H
#define DRIVE_H

#include "../../TerriBull/TerriBull.hpp"

using namespace TerriBull;

class TerriBull::Drive : public MechanicalComponent {
    protected:
    float * pCurrentAngle;
    Vector2* pCurrentPos;
    Vector2 pPreviousPos;
    float motorPowerThreshold = 1.0f; /* Should be tested */
    // Vector2 pCurrentError;
    // Vector2 pPreviousError;

    public:
    Drive() :  MechanicalComponent() {}
    void setAnglePtr(float * ptr) { this->pCurrentAngle = ptr; }
    void setPosPtr(Vector2* ptr) { this->pCurrentPos = ptr; }
    virtual ::std::string getType() = 0;
    virtual float* drive(Vector2 pos) = 0;
    virtual void change_orientation(float theta) = 0;
    virtual void reset() = 0;
    virtual void resultant_vector() = 0;
    virtual void tare_encoders() = 0;
};


#endif // DRIVE_H
