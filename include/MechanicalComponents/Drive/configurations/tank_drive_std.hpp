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
    Tank_Drive_Std(int portA, bool aReverse, int portB, bool bReverse, int portC, bool cReverse, int portD, bool dReverse, int portE,bool eReverse, int portF, bool fReverse, int gearSet) : TerriBull::Drive(gearSet), pA(portA), pB(portB), pC(portC), pD(portD), pE(portE), pF(portF), kPThetaTranslation(0.5) {
      this->pType = "Tank-Drive-Std";
      this->pMotorA = new pros::Motor(pA, (pros::motor_gearset_e)this->gearSet, aReverse);
      this->pMotorB = new pros::Motor(pB, (pros::motor_gearset_e)this->gearSet, bReverse);
      this->pMotorC = new pros::Motor(pC, (pros::motor_gearset_e)this->gearSet, cReverse);
      this->pMotorD = new pros::Motor(pD, (pros::motor_gearset_e)this->gearSet, dReverse);
      this->pMotorE = new pros::Motor(pE, (pros::motor_gearset_e)this->gearSet, eReverse);
      this->pMotorF = new pros::Motor(pF, (pros::motor_gearset_e)this->gearSet, fReverse);
      this->pMotorA->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorB->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorC->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorD->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorE->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorF->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->setPID(8.5, 0.01, 2.5);
      this->kPTheta = 2.5; this->kDTheta = 0.8;
      pMotorA->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorB->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorC->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorD->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    }
    ~Tank_Drive_Std();

    int drive(TerriBull::Vector2 pos);
    void reset();
    Vector2* resultant_vector();
    void tare_encoders()  {}
    int change_orientation(float theta);

};

#endif