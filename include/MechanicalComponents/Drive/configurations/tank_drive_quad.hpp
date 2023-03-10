/**
 * @file tank_drive_quad.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Tank Drive Driver configuration for a Quad Motor Drivebase
 *     
 * @version 0.1
 * @date 2023-03-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TANK_DRIVE_QUAD_H
#define TANK_DRIVE_QUAD_H
#include "../drive.hpp"
#include "../../pros/apix.h" /* bad program practice, TODO FIX */
#include <cmath>
#include <string>
#include <vector>

class Tank_Drive_Quad : public TerriBull::Drive {
    
    private:
    int pA, pB, pC, pD;
    pros::Motor* pMotorA, *pMotorB, *pMotorC, *pMotorD;
    float kPThetaTranslation;
    int targetDirection;

    protected:

    public:
    void setVoltage(float* vals);
    Tank_Drive_Quad(int portA, bool aReverse, int portB, bool bReverse, int portC, bool cReverse, int portD, bool dReverse, int gearSet, float conversion, float radius, float kP_Pos, float KI_Pos, float KD_Pos, float kP_Theta, float kI_Theta, float kD_Theta) : targetDirection(0), TerriBull::Drive(gearSet, conversion, radius, 127, kP_Pos, KI_Pos, KD_Pos, kP_Theta, kI_Theta, kD_Theta), pA(portA), pB(portB), pC(portC), pD(portD), kPThetaTranslation(0.2) {
      this->pType = "Tank-Drive-Quad";
      this->pMotorA = new pros::Motor(pA, (pros::motor_gearset_e)this->gearSet, aReverse);
      this->pMotorB = new pros::Motor(pB, (pros::motor_gearset_e)this->gearSet, bReverse);
      this->pMotorC = new pros::Motor(pC, (pros::motor_gearset_e)this->gearSet, cReverse);
      this->pMotorD = new pros::Motor(pD, (pros::motor_gearset_e)this->gearSet, dReverse);
      this->pMotorA->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorB->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorC->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorD->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      // this->setPID(8.5, 1.95, 0.8);
      // this->kPTheta = 2.5; this->kDTheta = 0.8;
      pMotorA->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorB->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorC->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorD->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    }
    ~Tank_Drive_Quad();

    int drive(TerriBull::Vector2 pos, float delta);
    void reset();
    Vector2* resultant_vector();
    void tare_encoders()  {
      this->pMotorA->tare_position();
      this->pMotorB->tare_position();
      this->pMotorC->tare_position();
      this->pMotorD->tare_position();
    }
    int change_orientation(float theta, float delta);
    void maneuverAngle(float theta, float delta);

};

#endif