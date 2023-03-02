/**
 * @file x_drive.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief xdrive driver configuration
 *     
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../include/MechanicalComponents/Drive/configurations/x_drive.hpp"

X_Drive::~X_Drive() {
    delete this->pMotorA;
    delete this->pMotorB;
    delete this->pMotorC;
    delete this->pMotorD;
}

void X_Drive::setVoltage(float* vals)  {
/* Less than some threshold */ 
    float lt = vals[0];float lb = vals[1];float rt = vals[2];float rb = vals[3];
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
    this->pMotorA->move(lt);
    this->pMotorB->move(lb);
    this->pMotorC->move(rt);
    this->pMotorD->move(rb);

}

int X_Drive::drive(TerriBull::Vector2 pos) {
    /* Theta of desired Modified By our current Look Angle */
    float* vals = new float[4];
    float angle = pos.theta - (*(this->pCurrentAngle) - 90);
    angle = (angle<0) ? 360.0 + angle : angle;
    int x = int(round(angle/45)) % 8;
    int dir = 0;
    float pct = 0;

    this->currentError = (pos - *(this->pCurrentPos)).r;
    this->sumError+=currentError;
    /* Basic PID Equation */
    pct = kP*currentError + kI*this->sumError + kD*this->dError();


    // // pros::lcd::set_text(4,::std::to_string(x) );
    switch(x) {
      case 0:
      case 4:
        dir = (angle < 90) ? 1 : -1;
        vals[0] = pct*dir; vals[1] = -pct*dir; vals[2] = pct*dir; vals[3] = pct*-dir;
        break;
      case 1:
      case 5:
        dir = (angle < 125) ? 1 : -1;
        vals[0] = pct*dir; vals[1] = 0; vals[2] = 0; vals[3] = pct*dir;
        break;
      case 2:
      case 6:
        dir = (angle < 180) ? 1 : -1;
        vals[0] = pct*dir; vals[1] = pct*dir; vals[2] = pct*dir; vals[3] = pct*dir;
        break;
      case 3:
      case 7:
        dir = (angle < 215) ? 1 : -1;
        vals[0] = 0; vals[1] = -pct*dir; vals[2] = -pct*dir; vals[3] = 0;
        break;
        default:
          delete[] vals;
          return -1;
    }
    this->setVoltage(vals);
    this->previousError = this->currentError;
    delete[] vals;
    return 0;
}

int X_Drive::change_orientation(float theta) {
  float* vals = new float[4];
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));
  this->sumError += this->currentError;
  float pwr = this->kPTheta * this->currentError + this->kI * this->sumError + this->kDTheta * this->dError();
  vals[0] = -pwr * fabs(this->currentError)/this->currentError;
  vals[1] = 0; 
  vals[2] = pwr * fabs(this->currentError)/this->currentError;
  vals[3] = 0;
  this->setVoltage(vals);
  delete[] vals;
  return 0;
}

void X_Drive::reset() {
    this->pMotorA->move(0);
    this->pMotorB->move(0);
    this->pMotorC->move(0);
    this->pMotorD->move(0);
    this->currentError = 0;
    this->sumError = 0;
    this->previousError = 0;
}
