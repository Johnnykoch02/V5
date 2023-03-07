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
#include "../../include/MechanicalComponents/Drive/configurations/tank_drive_quad.hpp"
#include <sstream>
#include <iomanip>
Tank_Drive_Quad::~Tank_Drive_Quad() {
    delete this->pMotorA;
    delete this->pMotorB;
    delete this->pMotorC;
    delete this->pMotorD;
}

void Tank_Drive_Quad::setVoltage(float* vals)  {
/* Less than some threshold */ 
    float lt = vals[0]; float lb = vals[1];float rt = vals[2]; float rb = vals[3];
    if (fabs(lt) > motorPowerThreshold) lt = motorPowerThreshold * fabs(lt) / lt;
    if (fabs(rt) > motorPowerThreshold) rt = motorPowerThreshold * fabs(rt) / rt;
    if (fabs(lb) > motorPowerThreshold) lb = motorPowerThreshold * fabs(lb) / lb;
    if (fabs(rb) > motorPowerThreshold) rb = motorPowerThreshold * fabs(rb) / rb;
    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(2);
    s3 << "Left: "<< lt << " Right: " << rt;
    pros::lcd::set_text(5, s3.str());
    this->pMotorA->move(lt);
    this->pMotorB->move(lb);
    this->pMotorC->move(rt);
    this->pMotorD->move(rb);
}

int Tank_Drive_Quad::drive(TerriBull::Vector2 pos) {
    /* Theta of desired Modified By our current Look Angle */
    float* vals = new float[4];
    float pct = 0;
    Vector2* dP = (pos - *(this->pCurrentPos));
    // float difToZero = fabs(GetDTheta(*(this->pCurrentAngle), RAD2DEG(dP->theta)));
    // float difToBack = fabs(GetDTheta(fmod(*(this->pCurrentAngle) + 180, 360), RAD2DEG(dP->theta)));
    // float difToZero = fabs(RAD2DEG(dP->theta) - 90);
    // float difToBack = fabs(RAD2DEG(dP->theta) - 270);
    int errorMod = (dP->theta > 215 && dP->theta < 345)? -1 : 1;
    this->currentError = dP->r * errorMod; /* First Part: Absolute Displacement, Second Part: Positive or Negative */
    this->sumError+=currentError;
    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(1);
    s3 << "Err: "<< this->currentError << " Mod: " << errorMod << "|" << dP->theta << " " << dP->r;
    pros::lcd::set_text(4,s3.str());
    /* Basic PID Equation */
    pct = kP*currentError + kI*this->sumError + kD*this->dError();
    pct*=errorMod;
    float offTrack = GetDTheta(dP->theta, *(this->pCurrentAngle)) * dP->r;
    int dir = fabs(offTrack)/offTrack;
    float pL = pct + MIN(fabs(this->kPThetaTranslation*offTrack), 25) * dir * errorMod;
    float pR = pct - MIN(fabs(this->kPThetaTranslation*offTrack), 25) * dir * errorMod;
    vals[0] = pL;
    vals[1] = pL;
    vals[2] = pR;
    vals[3] = pR;
    this->setVoltage(vals);
    delete[] vals;
    delete dP;    
    return 0;
}

int Tank_Drive_Quad::change_orientation(float theta) {
  float* vals = new float[6];
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));
  this->sumError += this->currentError;
  float pwr = this->kPTheta * this->currentError + this->kI * this->sumError + this->kDTheta * this->dError();
  std::stringstream s3;
  s3 << std::fixed << ::std::setprecision(1);
  s3 << "Err: "<< this->currentError << " Pwr: " << pwr;
  pros::lcd::set_text(4,s3.str());
  vals[0] = -pwr;
  vals[1] = -pwr;
  vals[2] = -pwr;
  vals[3] = pwr;
  vals[4] = pwr;
  vals[5] = pwr;
  this->setVoltage(vals);
  delete[] vals;
  return 0;
}

void Tank_Drive_Quad::reset() {
    this->pMotorA->move(0);
    this->pMotorB->move(0);
    this->pMotorC->move(0);
    this->pMotorD->move(0);
    this->currentError = 0;
    this->sumError = 0;
    this->previousError = 0;
}

Vector2* Tank_Drive_Quad::resultant_vector() {
    float l1 = this->pMotorA->get_position();
    float l2 = this->pMotorB->get_position();
    float r1 = this->pMotorC->get_position();
    float r2 = this->pMotorD->get_position();
    float left = ((l1 + l2) / 2) * this->wheelRadius * this->conversionFactor / ENCODER_UNIT[this->gearSet]; /* Assuming Radius of wheel is 5 */
    float right = ((r1 + r2) / 2) * this->wheelRadius * this->conversionFactor / ENCODER_UNIT[this->gearSet]; /* Assuming Radius of wheel is 5 */
    int leftDir = fabs(left) / left;
    int rightDir = fabs(right) / right;
    float leftAngle = (leftDir > 0) ? *(this->pCurrentAngle) : fmod((180 + *(this->pCurrentAngle)), 360.0);
    float rightAngle = (rightDir > 0)? *(this->pCurrentAngle) : fmod((180 + *(this->pCurrentAngle)), 360.0);
    Vector2* vecLeft = Vector2::polarToVector2(fabs(left), DEG2RAD(leftAngle));
    Vector2* vecRight = Vector2::polarToVector2(fabs(right), DEG2RAD(rightAngle));
    Vector2* vecUnNormalized = *vecLeft + *vecRight;
    Vector2* resultantVector = *vecUnNormalized * 0.5;
    
    /* Cleanup */
    this->tare_encoders();
    delete vecLeft;
    delete vecRight;
    delete vecUnNormalized;
    return resultantVector;    
}
