/**
 * @file MechanicalSystem.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Controller of all physical components on the BullBot
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MECHANICAL_SYSTEM_H
#define MECHANICAL_SYSTEM_H

#include "TerriBull/TerriBull.hpp"
#include "pros/imu.hpp"
#include <map>

using namespace TerriBull;


class TerriBull::MechanicalSystem {
    private:
        ::pros::Imu* pImu;
        TerriBull::Drive * pDrive;
        TerriBull::Vector2 pPosition;
        float * pAngle;
        float pStartingAngle;
        
    public:

    MechanicalSystem(int _imu, Drive * _drive)  {
        /*IMU Setup*/
        this->pImu = new ::pros::Imu(_imu);
        /*Drive Setup*/
        this->pDrive = _drive;
        this->pDrive->setAnglePtr(this->pAngle);
        this->pDrive->setPosPtr(&this->pPosition);

    }

    void GoToPosition(float x, float y);
    void TurnToAngle(float theta);


    float getAngle();
    
    void setStartingAngle(float angle) {
        this->pStartingAngle = angle;
    }
    
    void setStartingPosition(float x, float y) {
        this->pPosition = Vector2::cartesianToVector2(x, y);
    }

    /* Tasking Specific */
    float getDriveError() const {
        return this->pDrive->dError();
    }

    Vector2 getPosition() {
        return this->pPosition;
    }

    void update();

};

void MechanicalSystem::GoToPosition(float x, float y) {
    this->pDrive->drive(Vector2::cartesianToVector2(x, y));
}

float MechanicalSystem::getAngle() {
  float theta = this->pImu->get_heading(); /*TODO: Change hard Coded 90 to be a parsed variable */
  *(this->pAngle) = ::std::fmod(((360 - theta) + this->pStartingAngle), 360.0);
  return *(this->pAngle);
}

void update() {
    this->getAngle();
}


#endif