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
      this->pMotorA = new pros::Motor(pA);
      this->pMotorB = new pros::Motor(pB);
      this->pMotorC = new pros::Motor(pC);
      this->pMotorD = new pros::Motor(pD);
      this->setPID(0.5, 0.2, 0.3);
      pMotorA->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorB->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorC->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      pMotorD->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    }
    ~X_Drive();

    float* drive(TerriBull::Vector2 pos);
    // float dError();
      
    void reset();
    void resultant_vector() {}
    void tare_encoders()  {}


    void change_orientation(float theta);

    // void resultant_vector();
    // void tare_encoders();
    

};

#endif
