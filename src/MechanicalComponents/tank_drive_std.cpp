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
#include "../../include/MechanicalComponents/Drive/configurations/tank_drive_std.hpp"

Tank_Drive_Std::~Tank_Drive_Std() {
    delete this->pMotorA;
    delete this->pMotorB;
    delete this->pMotorC;
    delete this->pMotorD;
    delete this->pMotorE;
    delete this->pMotorF;
}

void Tank_Drive_Std::setVoltage(float* vals)  {
/* Less than some threshold */ 
    float lt = vals[0]; float lm = vals[1];float lb = vals[2]; float rt = vals[3]; float rm = vals[4]; float rb = vals[5];
    if (fabs(lt) < motorPowerThreshold && fabs(rt) < motorPowerThreshold && 
        fabs(lb) < motorPowerThreshold && fabs(rb) < motorPowerThreshold) {
      lt = lb = rt = rb = lm = rm = 0;
    }
    if (fabs(lt) > motorPowerThreshold) lt = motorPowerThreshold * fabs(lt) / lt;
    if (fabs(rt) > motorPowerThreshold) rt = motorPowerThreshold * fabs(rt) / rt;
    if (fabs(lm) > motorPowerThreshold) lm = motorPowerThreshold * fabs(lm) / lm;
    if (fabs(rm) > motorPowerThreshold) rm = motorPowerThreshold * fabs(rm) / rm;
    if (fabs(lb) > motorPowerThreshold) lb = motorPowerThreshold * fabs(lb) / lb;
    if (fabs(rb) > motorPowerThreshold) rb = motorPowerThreshold * fabs(rb) / rb;
    this->pMotorA->move(lt);
    this->pMotorB->move(lm);
    this->pMotorC->move(lb);
    this->pMotorD->move(rt);
    this->pMotorE->move(rm);
    this->pMotorF->move(rb);
}

int Tank_Drive_Std::drive(TerriBull::Vector2 pos) {
    /* Theta of desired Modified By our current Look Angle */
    float* vals = new float[6];
    float pct = 0;
    Vector2 dP = (pos - *(this->pCurrentPos));

    this->currentError = dP.r * sin(DEG2RAD(dP.theta)); /* First Part: Absolute Displacement, Second Part: Positive or Negative */
    this->sumError+=currentError;
    /* Basic PID Equation */
    pct = kP*currentError + kI*this->sumError + kD*this->dError();
    float offTrack = GetDTheta(dP.theta, *(this->pCurrentAngle)) * dP.r;
    int dir = fabs(offTrack)/offTrack;
    float pL = pct + MIN(fabs(this->kPThetaTranslation*offTrack), 25) * dir;
    float pR = pct - MIN(fabs(this->kPThetaTranslation*offTrack), 25) * dir;
    vals[0] = pL;
    vals[1] = pL;
    vals[2] = pL;
    
    vals[3] = pR;
    vals[4] = pR;
    vals[5] = pR;
    this->setVoltage(vals);
    this->previousError = this->currentError;
    delete[] vals;
    return 0;
}

int Tank_Drive_Std::change_orientation(float theta) {
  float* vals = new float[4];
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));
  this->sumError += this->currentError;
  float pwr = this->kPTheta * this->currentError + this->kI * this->sumError + this->kDTheta * this->dError();
  vals[0] = -pwr * fabs(this->currentError)/this->currentError;
  vals[1] = -pwr * fabs(this->currentError)/this->currentError;
  vals[2] = -pwr * fabs(this->currentError)/this->currentError;
  
  vals[3] = pwr * fabs(this->currentError)/this->currentError;
  vals[4] = pwr * fabs(this->currentError)/this->currentError;
  vals[5] = pwr * fabs(this->currentError)/this->currentError;
  this->setVoltage(vals);
  delete[] vals;
  return 0;
}

void Tank_Drive_Std::reset() {
    this->pMotorA->move(0);
    this->pMotorB->move(0);
    this->pMotorC->move(0);
    this->pMotorD->move(0);
    this->currentError = 0;
    this->sumError = 0;
    this->previousError = 0;
}
