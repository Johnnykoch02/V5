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
        RoboController* motherSystem;
        ::pros::Imu* pImu;
        TerriBull::Drive * pDrive;
        TerriBull::Intake * pIntake;
        TerriBull::Shooter * pShooter;
        TerriBull::Roller* pRoller;
        TerriBull::Expansion* pExpansion;
        TerriBull::Vector2 * pPosition;
        double * pAngle;
        float pStartingAngle;

    public:

    MechanicalSystem(int _imu, TerriBull::Drive * _drive);
    float getAngle();
    
    void setStartingAngle(float angle) {
        this->pStartingAngle = angle;
    }
    
    TerriBull::Vector2* getPosition();
    void setStartingPosition(float x, float y);

    /* Tasking Specific */
    float getDriveError() const;
    float getDriveDError() const;
    float getRollerError() const;
    float getRollerDError() const;
    void Init();
    void update(float delta);
    /* API TO Mechanical System */
    int GoToPosition(Vector2 pos);
    void resetDrive();
    int TurnToAngle(float theta);
    int turnOnIntake(float direction);
    int turnOffIntake();
    int spinRollerTo(float pos);
    int spinRollerFor(int direction, float time);
    int resetRoller();
    /*Setters*/
    void setMotherSystem(RoboController* _motherSystem);
    void setIntake(TerriBull::Intake * _intake);
    void setShooter(TerriBull::Shooter * _shooter);
    void setRoller(TerriBull::Roller * _roller);
    void setExpansion(TerriBull::Expansion * _expansion);
    /*Getters*/
    TerriBull::Intake * getIntake();
    TerriBull::Shooter * getShooter();
    TerriBull::Roller * getRoller();
    TerriBull::Expansion * getExpansion();
    TerriBull::Drive * getDrive();
    


};

#endif