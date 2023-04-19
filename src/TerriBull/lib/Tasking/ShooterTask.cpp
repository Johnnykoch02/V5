/**
 * @file ShooterTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot For Shooting Disks.
 *     
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../../../include/TerriBull/lib/Tasking/Types/ShooterTask.hpp"
ShooterTask::ShooterTask(ShooterType _shooterType, TerriBull::MechanicalSystem* _system) : Task(SHOOTER, _system), shooterType(_shooterType) {}

ShooterTask::~ShooterTask() {}

void ShooterTask::init() {
    this->finishedFlag = false;
    this->system->ResetShooter();
}

void ShooterTask::update(float delta) {
    if (!this->finishedFlag) {
    switch (this->shooterType) {
        case LOAD:
            this->system->LoadShooter();
            this->finishedFlag = this->system->isShooterLoaded();
            break;
        case SHOOT:
            this->system->ShootDisk();
            this->finishedFlag = this->system->isShotCompleted();
        }
    }   
}

void ShooterTask::terminate() {
    this->terminated = true;
    this->system->ResetShooter();
}