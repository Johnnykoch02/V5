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
    void setVoltage(float* vals);

    public:
    X_Drive(int portA, int portB, int portC, int portD) : TerriBull::Drive(), pA(portA), pB(portB), pC(portC), pD(portD) {
      this->pType = "X-Drive";
      this->pMotorA = new pros::Motor(pA, pros::E_MOTOR_GEARSET_18, false);
      this->pMotorB = new pros::Motor(pB, pros::E_MOTOR_GEARSET_18, false);
      this->pMotorC = new pros::Motor(pC, pros::E_MOTOR_GEARSET_18, true);
      this->pMotorD = new pros::Motor(pD, pros::E_MOTOR_GEARSET_18, true);
      this->setPID(0.5, 0.2, 0.3);
      this->kPTheta = 0.5;this->kDTheta = 0.2;
      pMotorA->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorB->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorC->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorD->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    }
    ~X_Drive();

    int drive(TerriBull::Vector2 pos);
    void reset();
    Vector2* resultant_vector() {return nullptr; }
    void tare_encoders()  {}

    int change_orientation(float theta);

};

#endif
