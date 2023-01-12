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

    /* Tasking Specific */
    float getDriveError() const {
        return this->pDrive->dError();
    }

};

void MechanicalSystem::GoToPosition(float x, float y) {
    this->pDrive->drive(Vector2::cartesianToVector2(x, y));
}

float MechanicalSystem::getAngle() {
  float theta = this->pImu->get_heading();
  *(this->pAngle) = ::std::fmod(((360 - theta) + 90), 360.0);
  return *(this->pAngle);
}


#endif