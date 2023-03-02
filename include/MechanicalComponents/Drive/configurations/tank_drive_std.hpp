/**
 * @file tank_drive_std.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Tank Drive Driver configuration
 *     
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TANK_DRIVE_STD_H
#define TANK_DRIVE_STD_H
#include "../drive.hpp"
#include "../../pros/apix.h" /* bad program practice, TODO FIX */
#include <cmath>
#include <string>
#include <vector>

class Tank_Drive_Std : public TerriBull::Drive {
    
    private:
    int pA, pB, pC, pD, pE, pF;
    pros::Motor* pMotorA, *pMotorB, *pMotorC, *pMotorD, *pMotorE, *pMotorF;
    float kPThetaTranslation;
    protected:
    void setVoltage(float* vals);

    public:
    Tank_Drive_Std(int portA, int portB, int portC, int portD, int portE, int portF) : TerriBull::Drive(), pA(portA), pB(portB), pC(portC), pD(portD), pE(portE), pF(portF), kPThetaTranslation(0.7) {
      this->pType = "Tank-Drive-Std";
      this->pMotorA = new pros::Motor(pA);
      this->pMotorB = new pros::Motor(pB);
      this->pMotorC = new pros::Motor(pC);
      this->pMotorD = new pros::Motor(pD);
      this->pMotorE = new pros::Motor(pE);
      this->pMotorF = new pros::Motor(pF);
      this->setPID(2.5, 2.2, 1.3);
      this->kPTheta = 0.5;this->kDTheta = 0.2;
      pMotorA->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorB->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorC->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorD->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    }
    ~Tank_Drive_Std();

    int drive(TerriBull::Vector2 pos);
    void reset();
    void resultant_vector() {}
    void tare_encoders()  {}
    int change_orientation(float theta);

    // void resultant_vector();
    // void tare_encoders();
    

};

#endif