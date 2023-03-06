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

class TerriBull::Drive : public TerriBull::MechanicalComponent {
    protected:
    float * pCurrentAngle;
    Vector2* pCurrentPos;
    Vector2 pPreviousPos;
    float motorPowerThreshold; /* Should be tested */
    float kPTheta, kDTheta;
    // Vector2 pCurrentError;
    // Vector2 pPreviousError;
    virtual void setVoltage(float* vals) = 0;
    public:
    Drive(int gearSet) : TerriBull::MechanicalComponent(gearSet), motorPowerThreshold(127), kPTheta(0), kDTheta(0) {}
    virtual void setAnglePtr(float * ptr) final { this->pCurrentAngle = ptr; }
    virtual void setPosPtr(Vector2* ptr) final { this->pCurrentPos = ptr; }
    virtual int drive(Vector2 pos) = 0;
    virtual int change_orientation(float theta) = 0;
    virtual void reset() = 0;
    virtual Vector2* resultant_vector() = 0;
    virtual void tare_encoders() = 0;
};     

#endif // DRIVE_H
