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

    MechanicalSystem(int _imu, TerriBull::Drive * _drive);

    void GoToPosition(float x, float y);
    void TurnToAngle(float theta);


    float getAngle();
    
    void setStartingAngle(float angle) {
        this->pStartingAngle = angle;
    }
    
    void setStartingPosition(float x, float y);

    /* Tasking Specific */
    float getDriveError() const;

    float getDriveDError() const;

    TerriBull::Vector2 getPosition();

    void update();

    /* API TO Mechanical System */

};

#endif