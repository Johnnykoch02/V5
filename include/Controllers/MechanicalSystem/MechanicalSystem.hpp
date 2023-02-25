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
#include "../../TerriBull/ProsAPI.h"
#include <map>

// using namespace TerriBull;


class TerriBull::MechanicalSystem {
    private:
        ::pros::Imu* pImu;
        TerriBull::Drive * pDrive;
        TerriBull::Vector2 pPosition;
        float * pAngle;
        float pStartingAngle;

    public:

    MechanicalSystem(int _imu, TerriBull::Drive * _drive)  {
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
        this->pPosition = TerriBull::Vector2::cartesianToVector2(x, y);
    }

    /* Tasking Specific */
    float getDriveError() const {
        return this->pDrive->dError();
    }

    TerriBull::Vector2 getPosition() {
        return this->pPosition;
    }

    void update();

    /* API TO Mechanical System */

};

void TerriBull::MechanicalSystem::GoToPosition(float x, float y) {
    Vector2 v = TerriBull::Vector2::cartesianToVector2(x, y);
    float* voltages = this->pDrive->drive(v);
    pros::lcd::set_text(7, "Voltages: " + ::std::to_string(voltages[0]) + " " + ::std::to_string(voltages[1]) + " " + ::std::to_string(voltages[2]) + " " + ::std::to_string(voltages[3]));
    // pros::lcd::set_text(7, "Voltages: " + ::std::to_string(int(voltages)));
    pros::lcd::set_text(6, this->pDrive->getType());
    if(this->pDrive->getType() == "x_drive" ) {
        pros::lcd::set_text(6, "Moving Xdrive");
        x_drive_motors_set_voltages(voltages[0], voltages[1], voltages[2], voltages[3]);
    }

    delete voltages;
}

float TerriBull::MechanicalSystem::getAngle() {
  float theta = this->pImu->get_heading(); /*TODO: Change hard Coded 90 to be a parsed variable */
  *(this->pAngle) = ::std::fmod(((360 - theta) + this->pStartingAngle), 360.0);
  return *(this->pAngle);
}

void TerriBull::MechanicalSystem::update() {
    this->getAngle();
}


#endif