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
MechanicalSystem::MechanicalSystem(int _imu, TerriBull::Drive* _drive, bool isAutoShoot) : pIntake(nullptr), pShooter(nullptr), pRoller(nullptr), pExpansion(nullptr), pIsAutoShoot(isAutoShoot) {
    this->pAngle = new double;
    /*IMU Setup*/
    this->pImu = new pros::Imu(_imu);
    this->pImu->tare();
    mu.tare();
    pros::delay(2000);
    /*Drive Setup*/
    this->pDrive = _drive;
    /*Initalize*/
    this->pThetaFilter = new KalmanFilter1D(0, 0.2, 0.1);
    this->targetGameObj = nullptr;

}

void MechanicalSystem::Init() {
    this->pDrive->setAnglePtr(this->pAngle);
    this->pDrive->setPosPtr(this->pPosition);
}

void MechanicalSystem::setStartingPosition(float x, float y){
    this->pPosition = TerriBull::Vector2::cartesianToVector2(x, y);
}

void MechanicalSystem::setTargetDeltaShootingAngle(float deltaShootingAngle) {
    this->targetDeltaShootingAngle = deltaShootingAngle;
}

/* Tasking Specific */
float  MechanicalSystem::getDriveError() const {
    return this->pDrive->getError();

}
float  MechanicalSystem::getDriveDError() const {
    return this->pDrive->ROdError();
}

bool MechanicalSystem::DriveNeedsAngleCorrection() const {
    return this->pDrive->needsAngleCorrection();
}

float  MechanicalSystem::getRollerError() const {
    if (this->pRoller != nullptr) return this->pRoller->getError();
    return 0;
}

float  MechanicalSystem::getRollerDError() const {
    if (this->pRoller != nullptr) return this->pDrive->ROdError();
    return 0;
}

float TerriBull::MechanicalSystem::getIntakeRPM() const {
    if (this->pIntake!= nullptr) {
        return this->pIntake->getRPM();
    } return 0;
}

float TerriBull::MechanicalSystem::getShooterRPM() const {
    if (this->pIntake!= nullptr) {
        return this->pIntake->getRPM();
    } return 0;
}

float TerriBull::MechanicalSystem::getRollerRPM() const {
    if (this->pIntake!= nullptr) {
        return this->pIntake->getRPM();
    } return 0;
}

float TerriBull::MechanicalSystem::getDriveRPM() const {
    if (this->pIntake!= nullptr) {
        return this->pIntake->getRPM();
    } return 0;
}

bool MechanicalSystem::isShotCompleted() const {
    if (this->pShooter!= nullptr) return this->pShooter->shotCompleted();
    return true;
}

bool MechanicalSystem::isShooterLoaded() const {
    if (this->pShooter!= nullptr) return this->pShooter->isLoaded();
    return true;
}

bool MechanicalSystem::isRollerCompleted() const {
    if (this->pShooter!= nullptr) return this->pRoller->timeFlag;
    return true;
}

bool MechanicalSystem::isRollerToggled() const {
    if(this->pRoller != nullptr) return this->pRoller->getToggled();
    return false;
}
bool MechanicalSystem::isIntakeToggled() const {
    if(this->pIntake != nullptr) return this->pIntake->getToggled();
    return false;
}
bool MechanicalSystem::isDriveToggled() const {
    if(this->pDrive != nullptr) return this->pDrive->getToggled();
    return false;
}
bool MechanicalSystem::isShooterToggled() const {
    if(this->pShooter != nullptr) return this->pShooter->getToggled();
    return false;
}
bool MechanicalSystem::isRollerReset() const {
    if(this->pRoller != nullptr) return this->pRoller->isReset(); 
    return false;
}
bool MechanicalSystem::isIntakeReset() const {
    if(this->pIntake != nullptr) return this->pIntake->isReset(); 
    return false;
}
bool MechanicalSystem::isDriveReset() const {
    if(this->pRoller != nullptr) return this->pDrive->isReset(); 
    return false;
}
bool MechanicalSystem::isShooterReset() const {
    if(this->pRoller != nullptr) return this->pShooter->isReset(); 
    return false;
}

TerriBull::Vector2*  MechanicalSystem::getPosition() {
    return this->pPosition;
}

float TerriBull::MechanicalSystem::getTargetDeltaShootingAngle() const {
    return this->targetDeltaShootingAngle;
}
bool TerriBull::MechanicalSystem::getIsAutoShoot() const {
    return this->pIsAutoShoot;
}
void TerriBull::MechanicalSystem::ResetDrive() {
    this->pDrive->reset();
}

float TerriBull::MechanicalSystem::getAngle() {
  float theta = mu.get_heading(); 
//   if( this->motherSystem) this->pThetaFilter->update(theta, this->motherSystem->delta());
//   this->pThetaFilter->predict();
//   theta = this->pThetaFilter->getState()[0];
  *(this->pAngle) = ::std::fmod(((360 - theta) + this->pStartingAngle), 360.0);//90;//
  return *(this->pAngle);
}

void TerriBull::MechanicalSystem::Update(float delta) {
    this->getAngle();
    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(1);
    s3 << "Ang:" << *(this->pAngle) << "|Pos:x->" << this->pPosition->x << "y->" << this->pPosition->y;
    //pros::lcd::set_text(3, s3.str());
    // std::stringstream s4;

    Vector2* dP = this->pDrive->resultant_vector();
    this->pPosition->x += dP->x;
    this->pPosition->y += dP->y;
    delete dP;
}

/* MECHANICAL SYSTEM API FUNCTIONS */
int TerriBull::MechanicalSystem::GoToPosition(Vector2 v_f, Vector2 v_i, bool reverse) {
    return this->pDrive->drive(v_f, v_i, this->motherSystem->delta(), reverse);
}

int TerriBull::MechanicalSystem::TurnToAngle(float theta) {
     return this->pDrive->change_orientation(theta, this->motherSystem->delta());
}

int TerriBull::MechanicalSystem::TurnOnIntake(float direction) {
    if (this->pIntake != nullptr) {
        return this->pIntake->TurnOn(direction);
    } return -1; 
}

int TerriBull::MechanicalSystem::TurnOffIntake() {
    if (this->pIntake != nullptr) {
        return this->pIntake->TurnOff();
    } return -1;
}

int TerriBull::MechanicalSystem::TurnOnRoller(float pct0fMax) {
    if (this->pRoller!= nullptr) {
        return this->pRoller->TurnOn(pct0fMax);
    } return -1;
}

int TerriBull::MechanicalSystem::TurnOffRoller() {
    if (this->pRoller!= nullptr) {
        return this->pRoller->TurnOff();
    } return -1;
}

int TerriBull::MechanicalSystem::SpinRollerTo(float pos) {
    if (this->pRoller != nullptr) {
        return this->pRoller->SpinToPos(pos);
    } return -1;
}

int TerriBull::MechanicalSystem::SpinRollerFor(int direction, float time) {
    if (this->pRoller != nullptr) {
        return this->pRoller->Spin(direction, time, this->motherSystem->delta());
    } return -1;
}

int TerriBull::MechanicalSystem::ResetRoller() {
    if (this->pRoller != nullptr) {
        this->pRoller->reset();
        return 0;
    } return -1;
}

int TerriBull::MechanicalSystem::ShootDisk() {
    if (this->pShooter != nullptr) {
        return this->pShooter->Shoot(this->motherSystem->delta(), nullptr);
    } return -1;
}

int TerriBull::MechanicalSystem::LoadShooter() {
    if (this->pShooter != nullptr) {
        return this->pShooter->Load(this->motherSystem->delta(), nullptr);
    } return -1;
}

int TerriBull::MechanicalSystem::TurnOnShooter() {
    if (this->pShooter != nullptr) {
        return this->pShooter->turnOn();
    } return -1;
}

int TerriBull::MechanicalSystem::ResetShooter() {
    if (this->pShooter != nullptr) {
        return this->pShooter->reset();
    } return -1;
}

int TerriBull::MechanicalSystem::ConstrictMotorGroupCurrent(TerriBull::MechanicalComponent::MotorRefs* refGroup) {
    /* TODO: Log the Constriction to the Log File refGroup.ComponentName */
    for (int i = 0; i < refGroup->NumMotors; i++) {
        if (refGroup->Motors[i]!= nullptr) {
            refGroup->Motors[i]->set_current_limit(1000);
        }
    }
    return 0;
}

int TerriBull::MechanicalSystem::UnConstrictMotorGroupCurrent(TerriBull::MechanicalComponent::MotorRefs* refGroup) {
    /* TODO: Log the Un-Constriction to the Log File refGroup.ComponentName */
    for (int i = 0; i < refGroup->NumMotors; i++) {
        if (refGroup->Motors[i]!= nullptr) {
            refGroup->Motors[i]->set_current_limit(2500);
        }
    }
    return 0;
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
void TerriBull::MechanicalSystem::setTargetObject(TerriBull::GameObject * _targetGameObject) {
    this->targetGameObj = _targetGameObject;
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
TerriBull::GameObject * TerriBull::MechanicalSystem::getTargetObject() {
    return this->targetGameObj;
}
