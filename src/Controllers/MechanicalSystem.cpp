/**
 * @file MechanicalSystem.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Controller of all physical components on the BullBot
 *     
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../include/Controllers/MechanicalSystem/MechanicalSystem.hpp"
MechanicalSystem::MechanicalSystem(int _imu, TerriBull::Drive* _drive) {
    this->pAngle = new float;
    /*IMU Setup*/
    this->pImu = new ::pros::Imu(_imu);
    /*Drive Setup*/
    this->pDrive = _drive;
    this->pDrive->setAnglePtr(this->pAngle);
    this->pDrive->setPosPtr(this->pPosition);
}

void MechanicalSystem::setStartingPosition(float x, float y){
    this->pPosition = TerriBull::Vector2::cartesianToVector2(x, y);
}

float  MechanicalSystem::getDriveError() const {
    return this->pDrive->getError();

}
float  MechanicalSystem::getDriveDError() const {
    return this->pDrive->dError();
}

TerriBull::Vector2  MechanicalSystem::getPosition(){
    return this->pPosition;
}

void TerriBull::MechanicalSystem::GoToPosition(float x, float y) {
    Vector2 *v = TerriBull::Vector2::cartesianToVector2(x, y);
    this->pDrive->drive(*v);
    delete v;
}

void TerriBull::MechanicalSystem::resetDrive() {
    this->pDrive->reset();
}

float TerriBull::MechanicalSystem::getAngle() {
  float theta = this->pImu->get_heading(); /*TODO: Change hard Coded 90 to be a parsed variable */
  *(this->pAngle) = ::std::fmod(((360 - theta) + this->pStartingAngle), 360.0);
  return *(this->pAngle);
}

void TerriBull::MechanicalSystem::update() {
    this->getAngle();
}
