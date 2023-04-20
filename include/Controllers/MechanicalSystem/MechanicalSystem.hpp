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

#include "../../TerriBull/TerriBull.hpp"
#include "../../TerriBull/lib/KalmanFilter.hpp"
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
        KalmanFilter1D* pThetaFilter;
        GameObject* targetGameObj;
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
    bool DriveNeedsAngleCorrection() const;
    float getRollerError() const;
    float getRollerDError() const;
    bool isShotCompleted() const;
    bool isShooterLoaded() const;
    bool isRollerCompleted() const;
    // bool isExpansionCompleted() const;
    bool isRollerToggled() const;
    bool isIntakeToggled() const;
    bool isDriveToggled() const;
    bool isShooterToggled() const;
    bool isRollerReset() const;
    bool isIntakeReset() const;
    bool isDriveReset() const;
    bool isShooterReset() const;
    
    void Init();
    void update(float delta);

    /* API To Mechanical System */
    int GoToPosition(Vector2 v_f, Vector2 v_i, bool reverse);
    void ResetDrive();
    int TurnToAngle(float theta);
    int TurnOnIntake(float direction); //+
    int TurnOffIntake();
    int SpinRollerTo(float pos);
    int SpinRollerFor(int direction, float time);
    int ResetRoller();
    int ShootDisk();
    int LoadShooter();
    int TurnOnShooter();
    int ResetShooter();
    int ConstrictMotorGroupCurrent(TerriBull::MechanicalComponent::MotorRefs* refGroup);
    int UnConstrictMotorGroupCurrent(TerriBull::MechanicalComponent::MotorRefs* refGroup);


    float getIntakeRPM() const;
    float getShooterRPM() const;
    float getRollerRPM() const;
    float getDriveRPM() const;

    /*Setters*/
    void setMotherSystem(RoboController* _motherSystem);
    void setIntake(TerriBull::Intake * _intake);
    void setShooter(TerriBull::Shooter * _shooter);
    void setRoller(TerriBull::Roller * _roller);
    void setExpansion(TerriBull::Expansion * _expansion);
    void setTargetObject(GameObject* _targetGameObject);
    /*Getters*/
    TerriBull::Intake * getIntake();
    TerriBull::Shooter * getShooter();
    TerriBull::Roller * getRoller();
    TerriBull::Expansion * getExpansion();
    TerriBull::Drive * getDrive();
    TerriBull::GameObject * getTargetObject();

};

#endif