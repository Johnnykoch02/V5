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
    double * pCurrentAngle;
    float conversionFactor;
    float wheelRadius;
    float maxSpeed;
    Vector2* pCurrentPos;
    Vector2 pPreviousPos;
    float motorPowerThreshold; /* Should be tested */
    float kPTheta, kITheta, kDTheta;
    // Vector2 pCurrentError;
    // Vector2 pPreviousError;
    public:
    Drive(int gearSet, float _conversionFactor, float _wheelRadius, float maxSpeed, float kP_Pos, float KI_Pos, float KD_Pos, float kP_Theta, float kI_Theta, float kD_Theta) : TerriBull::MechanicalComponent(gearSet), motorPowerThreshold(127), maxSpeed(127), conversionFactor(_conversionFactor), wheelRadius(_wheelRadius), kPTheta(kP_Theta), kITheta(kI_Theta), kDTheta(kD_Theta) {
        this->kP = kP_Pos;
        this->kI = KI_Pos;
        this->kD = KD_Pos;
    }
    virtual void setVoltage(float* vals) = 0;
    virtual void setAnglePtr(double * ptr) final { this->pCurrentAngle = ptr; }
    virtual void setPosPtr(Vector2* ptr) final { this->pCurrentPos = ptr; }
    virtual void setMaxSpeed(float maxSpeed) final { this->maxSpeed = maxSpeed; }
    virtual float* getRefMaxSpeed() final { return &this->maxSpeed; }
    virtual int drive(Vector2 pos, float delta) = 0;
    virtual int change_orientation(float theta, float delta) = 0;
    virtual void reset() = 0;
    virtual Vector2* resultant_vector() = 0;
    virtual void tare_encoders() = 0;
};     

#endif // DRIVE_H
