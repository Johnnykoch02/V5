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
#include "../../../pros/motors.h"
#include "../../../main.h"
#include <cmath>
#include <vector>


using namespace TerriBull;


class X_Drive : public TerriBull::Drive {
    
    private:
    int pA, pB, pC, pD;

    public:
    X_Drive(int portA, int portB, int portC, int portD);
    ~X_Drive();

    void drive(TerriBull::Vector2 pos);

    void setVoltage(float lt, float lb, float rt, float rb);

    // float dError();
      
    void reset();
    void resultant_vector() {}
    void tare_encoders()  {}

    void change_orientation(float theta);

    // void resultant_vector();
    // void tare_encoders();
    

};

X_Drive::X_Drive(int portA, int portB, int portC, int portD) : TerriBull::Drive(), pA(portA), pB(portB), pC(portC), pD(portD) {

   
    this->setPID(0.5, 0.2, 0.3);

    pMotorA.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    pMotorB.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    pMotorC.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    pMotorD.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

X_Drive::~X_Drive() {
    // delete pMotorA;
    // delete pMotorB;
    // delete pMotorC;
    // delete pMotorD;
}

void X_Drive::drive(TerriBull::Vector2 pos) {
    /* Theta of desired      Modified By our current Look Angle */

    float angle = pos.theta - (*(this->pCurrentAngle) - 90);
    angle = (angle<0) ? 360.0 + angle : angle;
    int x = int(round(angle/45)) % 8;
    int dir = 0;
    float pct = 0;

    this->currentError = (pos - *(this->pCurrentPos)).r;

    /* Basic PID Equation */
    pct = kP*currentError + kI*currentError*currentError + kD*this->dError();


    // // pros::lcd::set_text(4,::std::to_string(x) );
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
          // ::pros::lcd::set_text(4, "ERROR" );s
          break;
    }

    this->previousError = this->currentError;
}

void X_Drive::change_orientation(float theta) {
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));
  float pct = kP*this->currentError + kI*this->currentError*this->currentError + kD*this->dError();
  int dir = this->currentError/fabs(this->currentError);
  this->setVoltage(pct*dir, 0, 0, -pct*dir);
  this->previousError = this->currentError;
}

void  X_Drive::setVoltage(float lt, float lb, float rt, float rb)  {
  /* Less than some threshold */ 
  if (fabs(lt) < motorPowerThreshold && fabs(rt) < motorPowerThreshold && 
      fabs(lb) < motorPowerThreshold && fabs(rb) < motorPowerThreshold) {
    lt = lb = rt = rb = 0;
  }
  else {
  }
  if (fabs(lt) > motorPowerThreshold) lt = motorPowerThreshold * fabs(lt) / lt;
  if (fabs(rt) > motorPowerThreshold) rt = motorPowerThreshold * fabs(rt) / rt;
  if (fabs(lb) > motorPowerThreshold) lb = motorPowerThreshold * fabs(lb) / lb;
  if (fabs(rb) > motorPowerThreshold) rb = motorPowerThreshold * fabs(rb) / rb;

  pros::c::motor_move(this->pA, lt);
  pros::c::motor_move(this->pB, lb);
  pros::c::motor_move(this->pC, rt);
  pros::c::motor_move(this->pD, rb);

}

void X_Drive::reset() {
  this->currentError = this->previousError = 0;
  pros::c::motor_move(this->pA, 0);
  pros::c::motor_move(this->pB, 0);
  pros::c::motor_move(this->pC, 0);
  pros::c::motor_move(this->pD, 0);
}

#endif
