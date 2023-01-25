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
#include "../../TerriBull/TerriBull.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/llemu.hpp"
#include <math.h>
#include <vector>

float rx,ry;

class X_Drive : public TerriBull::Drive {
    
    typedef ::std::map<::pros::Motor*, TerriBull::Vector2> ErrorMap;

    private:
    pros::Motor * pMotorA; // -> Top Left
    pros::Motor * pMotorB; // -> Bottom Left
    pros::Motor * pMotorC; // -> Top Right
    pros::Motor * pMotorD; // -> Bottom Right


    X_Drive(int portA, int portB, int portC, int portD);

    void drive(TerriBull::Vector2 pos);

    void setVoltage(float lt, float lb, float rt, float rb);

    float dError();
      
    void reset();

    // void resultant_vector();

    // void tare_encoders();

};

X_Drive::X_Drive(int portA, int portB, int portC, int portD) {
    pMotorA = new pros::Motor(portA, pros::E_MOTOR_GEARSET_18, false);
    pMotorB = new pros::Motor(portB, pros::E_MOTOR_GEARSET_18, false);
    pMotorC = new pros::Motor(portC, pros::E_MOTOR_GEARSET_18, true);
    pMotorD = new pros::Motor(portD, pros::E_MOTOR_GEARSET_18, true);

}

X_Drive::~X_Drive() {
    delete pMotorA;
    delete pMotorB;
    delete pMotorC;
    delete pMotorD;
}

void X_Drive::drive(TerriBull::Vector2 pos) {
    /* Theta of desired      Modified By our current Look Angle */

    float angle = pos.theta - ((*this->pCurrentAngle) - 90);
    angle = (angle<0) ? 360.0 + angle : angle;
    int x = int(round(angle/45)) % 8;
    int dir = 0;
    float pct = 0;

    this->currentError = (pos - *(this->pCurrentPos)).r;

    /* Basic PID Equation */
    pct = kP*currentError + kI*currentError*currentError + kD*this->dError();


    pros::lcd::set_text(4, std::to_string(x) );
    switch(x) {
      case 0:
      case 4:
        dir = (angle < 90) ? 1 : -1;
        setVoltage(pct*dir, -pct*dir, pct*dir, pct*-dir);
        break;
      case 1:
      case 5:
        dir = (angle < 125) ? 1 : -1;
        setVoltage( dir*pct, 0, 0,dir*pct);
        break;
      case 2:
      case 6:
        dir = (angle < 180) ? 1 : -1;
        setVoltage( dir*pct, dir*pct, dir*pct,dir*pct);
        break;
      case 3:
      case 7:
        dir = (angle < 215) ? 1 : -1;
        setVoltage( 0, -dir*pct, -dir*pct,0);
        break;
        default:
          ::pros::lcd::set_text(4, "ERROR" );
          break;
    }

    this->previousError = this->currentError;
}

void X_Drive::change_orientation(float theta) {
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));

}

void  X_Drive::setVoltage(float lt, float lb, float rt, float rb)  {
  /* Less than some threshold */ 
  if (fabs(lt) < motorPowerThreshold && fabs(rt) < motorPowerThreshold && 
      fabs(lb) < motorPowerThreshold && fabs(rb) < motorPowerThreshold) {
    lt = lb = rt = rb = 0;
  }
  pMotorA->move_voltage(lt*this->pVoltageCap);
  pMotorB->move_voltage(rt*this->pVoltageCap);
  pMotorC->move_voltage(lb*this->pVoltageCap);
  pMotorD->move_voltage(rb*this->pVoltageCap);

}

void X_Drive::reset() {
  this->currentError = this->previousError = 0;
  pMotorA->move_voltage(0);
  pMotorB->move_voltage(0);
  pMotorC->move_voltage(0);
  pMotorD->move_voltage(0);
}

#endif
