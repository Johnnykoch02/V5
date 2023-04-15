/**
 * @file x_drive.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief xdrive driver configuration
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef X_DRIVE_H
#define X_DRIVE_H
#include "../drive.hpp"
#include "../../pros/apix.h" /* bad program practice, TODO FIX */
#include <cmath>
#include <string>
#include <vector>

class X_Drive : public TerriBull::Drive {
    
    private:
    int pA, pB, pC, pD;
    pros::Motor* pMotorA, *pMotorB, *pMotorC, *pMotorD;
    protected:

    public:
    void setVoltage(float* vals);
    X_Drive(int portA, int portB, int portC, int portD, int gearSet, float conversion, float radius, float _wheelBase, float kP_Pos, float KI_Pos, float KD_Pos, float kP_Theta, float kI_Theta, float kD_Theta) : TerriBull::Drive(gearSet, conversion, radius, _wheelBase, 127, kP_Pos, KI_Pos, KD_Pos, kP_Theta, kI_Theta, kD_Theta), pA(portA), pB(portB), pC(portC), pD(portD) {
      this->pType = "X-Drive";
      this->pMotorA = new pros::Motor(pA, (pros::motor_gearset_e)this->gearSet, false);
      this->pMotorB = new pros::Motor(pB, (pros::motor_gearset_e)this->gearSet, false);
      this->pMotorC = new pros::Motor(pC, (pros::motor_gearset_e)this->gearSet, true);
      this->pMotorD = new pros::Motor(pD, (pros::motor_gearset_e)this->gearSet, true);
      this->pMotorA->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorB->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorC->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      this->pMotorD->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
      pMotorA->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorB->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorC->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorD->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    }
    ~X_Drive();

    int drive(TerriBull::Vector2 v_f, TerriBull::Vector2 v_i, float delta, bool reverse);
    void reset();
    Vector2* resultant_vector();
    void tare_encoders()  {
      this->pMotorA->tare_position();
      this->pMotorB->tare_position();
      this->pMotorC->tare_position();
      this->pMotorD->tare_position();
    }

    int change_orientation(float theta, float delta);

};

#endif
