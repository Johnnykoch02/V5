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
    // Vector2 pCurrentError;
    // Vector2 pPreviousError;
    virtual void setVoltage(float* vals) {}
    public:
    Drive() : TerriBull::MechanicalComponent(), motorPowerThreshold(127) {}
    virtual void setAnglePtr(float * ptr) final { this->pCurrentAngle = ptr; }
    virtual void setPosPtr(Vector2* ptr) final { this->pCurrentPos = ptr; }
    virtual float* drive(Vector2 pos) { return nullptr; }
    virtual void change_orientation(float theta) {}
    virtual void reset() {}
    virtual void resultant_vector() {}
    virtual void tare_encoders() {}
};     

#endif // DRIVE_H
