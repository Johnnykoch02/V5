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
#include <sstream>
#include <iomanip>
MechanicalSystem::MechanicalSystem(int _imu, TerriBull::Drive* _drive) : pIntake(nullptr), pShooter(nullptr), pRoller(nullptr), pExpansion(nullptr) {
    this->pAngle = new double;
    /*IMU Setup*/
    this->pImu = new pros::Imu(_imu);
    this->pImu->tare();
    mu.tare();
    pros::delay(2000);
    /*Drive Setup*/
    this->pDrive = _drive;    
}

void MechanicalSystem::Init() {
    this->pDrive->setAnglePtr(this->pAngle);
    this->pDrive->setPosPtr(this->pPosition);
}

void MechanicalSystem::setStartingPosition(float x, float y){
    this->pPosition = TerriBull::Vector2::cartesianToVector2(x, y);
}

/* Tasking Specific */
float  MechanicalSystem::getDriveError() const {
    return this->pDrive->getError();

}
float  MechanicalSystem::getDriveDError() const {
    return this->pDrive->ROdError();
}

float  MechanicalSystem::getRollerError() const {
    if (this->pRoller != nullptr) return this->pRoller->getError();
    return 0;
}

float  MechanicalSystem::getRollerDError() const {
    if (this->pRoller != nullptr) return this->pDrive->ROdError();
    return 0;
}

bool MechanicalSystem::isShotCompleted() const {
    if (this->pShooter!= nullptr) return this->pShooter->shotCompleted();
    return true;
}

bool MechanicalSystem::isRollerCompleted() const {
    if (this->pShooter!= nullptr) return this->pRoller->timeFlag;
    return true;
}

TerriBull::Vector2*  MechanicalSystem::getPosition() {
    return this->pPosition;
}

void TerriBull::MechanicalSystem::resetDrive() {
    this->pDrive->reset();
}

float TerriBull::MechanicalSystem::getAngle() {
  float theta = mu.get_heading(); 
  *(this->pAngle) = ::std::fmod(((360 - theta) + this->pStartingAngle), 360.0);//90;//
  return *(this->pAngle);
}

void TerriBull::MechanicalSystem::update(float delta) {
    this->getAngle();
    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(1);
    s3 << "Ang:" << *(this->pAngle) << "|Pos:x->" << this->pPosition->x << "y->" << this->pPosition->y;
    pros::lcd::set_text(3, s3.str());
    // std::stringstream s4;

    Vector2* dP = this->pDrive->resultant_vector();
    this->pPosition->x += dP->x;
    this->pPosition->y += dP->y;
    delete dP;
}

/* MECHANICAL SYSTEM API FUNCTIONS */
int TerriBull::MechanicalSystem::GoToPosition(Vector2 pos) {
    return this->pDrive->drive(pos, this->motherSystem->delta());
}

int TerriBull::MechanicalSystem::TurnToAngle(float theta) {
     return this->pDrive->change_orientation(theta, this->motherSystem->delta());
}

int TerriBull::MechanicalSystem::turnOnIntake(float direction) {
    if (this->pIntake != nullptr) {
        return this->pIntake->TurnOn(direction);
    } return -1; 
}

int TerriBull::MechanicalSystem::turnOffIntake() {
    if (this->pIntake != nullptr) {
        return this->pIntake->TurnOff();
    } return -1;
}

int TerriBull::MechanicalSystem::spinRollerTo(float pos) {
    if (this->pRoller != nullptr) {
        return this->pRoller->SpinToPos(pos);
    } return -1;
}

int TerriBull::MechanicalSystem::spinRollerFor(int direction, float time) {
    if (this->pRoller != nullptr) {
        return this->pRoller->Spin(direction, time, this->motherSystem->delta());
    } return -1;
}

int TerriBull::MechanicalSystem::resetRoller() {
    if (this->pRoller != nullptr) {
        this->pRoller->reset();
        return 0;
    } return -1;
}

int TerriBull::MechanicalSystem::ShootDisk() {
    if (this->pShooter != nullptr) {
        return this->pShooter->Shoot(this->motherSystem->delta());
    } return -1;
}

int TerriBull::MechanicalSystem::resetShooter() {
    if (this->pShooter != nullptr) {
        return this->pShooter->reset();
    } return -1;
}

/*  SETTERS AND GETTERS  */
void TerriBull::MechanicalSystem::setMotherSystem(RoboController* _motherSystem) {
    this->motherSystem = _motherSystem;
}
void TerriBull::MechanicalSystem::setIntake(TerriBull::Intake * _intake) {
    this->pIntake = _intake;
}
void TerriBull::MechanicalSystem::setShooter(TerriBull::Shooter * _shooter) {
    this->pShooter = _shooter;
}
void TerriBull::MechanicalSystem::setRoller(TerriBull::Roller * _roller) {
    this->pRoller = _roller;
}
void TerriBull::MechanicalSystem::setExpansion(TerriBull::Expansion * _expansion) {
    this->pExpansion = _expansion;
}
TerriBull::Intake * TerriBull::MechanicalSystem::getIntake() {
    return this->pIntake;
}
TerriBull::Shooter * TerriBull::MechanicalSystem::getShooter() {
    return this->pShooter;
}
TerriBull::Roller * TerriBull::MechanicalSystem::getRoller() {
    return this->pRoller;
}
TerriBull::Expansion * TerriBull::MechanicalSystem::getExpansion() {
    return this->pExpansion;
}
TerriBull::Drive * TerriBull::MechanicalSystem::getDrive() {
    return this->pDrive;
}
