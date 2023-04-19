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
    if (fabs(lt) > this->maxSpeed) lt = this->maxSpeed * fabs(lt) / lt;
    if (fabs(rt) > this->maxSpeed) rt = this->maxSpeed * fabs(rt) / rt;
    if (fabs(lb) > this->maxSpeed) lb = this->maxSpeed * fabs(lb) / lb;
    if (fabs(rb) > this->maxSpeed) rb = this->maxSpeed * fabs(rb) / rb;
    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(2);
    s3 << "Left: "<< lt << " Right: " << rt;
    pros::lcd::set_text(5, s3.str());
    this->pMotorA->move(lt);
    this->pMotorB->move(lb);
    this->pMotorC->move(rt);
    this->pMotorD->move(rb);
}

int Tank_Drive_Quad::drive(TerriBull::Vector2 v_f, TerriBull::Vector2 v_i, float delta, bool reverse) {
    /* Theta of desired Modified By our current Look Angle */
    float* vals = new float[6];
    float pct = 0;
    Vector2* dP = (v_f - *(this->pCurrentPos));
    float difToZero = fabs(GetDTheta(RAD2DEG(dP->theta), *(this->pCurrentAngle)));
    float difToBack = fabs(GetDTheta(RAD2DEG(dP->theta), fmod(*(this->pCurrentAngle)+ 180, 360)));
    int errorMod = (difToZero < difToBack) ? 1 : -1;
    if (dP->r > errorMod) targetDirection = errorMod;
    this->currentError = dP->r * errorMod; /* First Part: Absolute Displacement, Second Part: Positive or Negative */
    this->sumError+=currentError;
    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(1);
    s3 << "Err: "<< this->currentError << " Mod: " << errorMod << "|" << RAD2DEG(dP->theta) << " " << dP->r;
    pros::lcd::set_text(4,s3.str());
    /* Basic PID Equation */
    pct = kP*currentError + kI*this->sumError + kD*this->dError() / delta;

    if (fabs(pct) >  this->maxSpeed) {/* Clmp Pwr to 127 */
        pct = this->maxSpeed * fabs(pct) / pct;
    }
    float pL = pct;
    float pR = pct;
    int angleMod = (errorMod > 0 ) ? 0 : 180;
    float offTrack = GetDTheta(RAD2DEG(dP->theta),  fmod(*(this->pCurrentAngle) + angleMod, 360));
    if (fabs(dP->r) > 3) {
        int dir = fabs(offTrack)/offTrack;
        pL *= 0.95;
        pR *= 0.95;
        pL += MIN(fabs(this->kPThetaTranslation*offTrack), fabs(0.1* pct)) * dir * errorMod;
        pR -= MIN(fabs(this->kPThetaTranslation*offTrack), fabs(0.1* pct)) * dir * errorMod;
    }
    else if (fabs(dP->r) > 1.0 && offTrack > 90) {
        this->maneuverAngle(fmod(RAD2DEG(dP->theta) + angleMod, 360), delta);
        delete[] vals;
        delete dP;
        return 0;
    }
    vals[0] = pL;
    vals[1] = pL;
    vals[2] = pR;
    vals[3] = pR;
    this->setVoltage(vals);
    delete[] vals;
    delete dP;    
    return 0;
}

int Tank_Drive_Quad::change_orientation(float theta, float delta) {
  float* vals = new float[6];
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));
  this->sumError += this->currentError;
  float pwr = this->kPTheta * this->currentError + this->kITheta * this->sumError + this->kDTheta * this->dError() / delta;
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

void Tank_Drive_Quad::maneuverAngle(float theta, float delta) {
  float* vals = new float[4];
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));
  this->sumError += this->currentError;
  float pwr = this->kPTheta * this->currentError + this->kI * this->sumError + this->kDTheta * this->dError() / delta;
  std::stringstream s3;
  s3 << std::fixed << ::std::setprecision(1);
  s3 << "Err: "<< this->currentError << " Pwr: " << pwr;
  pros::lcd::set_text(4,s3.str());
  vals[0] = -pwr;
  vals[1] = -pwr;
  vals[2] = pwr;
  vals[3] = pwr;
  this->setVoltage(vals);
  delete[] vals;
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
    float left = ((l1 + l2) / 2) * 2 * PI  * this->wheelRadius * this->conversionFactor / ENCODER_UNIT[this->gearSet]; /* Assuming Radius of wheel is 5 */
    float right = ((r1 + r2) / 2) * 2 * PI * this->wheelRadius * this->conversionFactor / ENCODER_UNIT[this->gearSet]; /* Assuming Radius of wheel is 5 */
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

float Tank_Drive_Quad::getRPM() const {
    float rpmI = fabs(this->pMotorA->get_actual_velocity());
    float rpmJ = fabs(this->pMotorB->get_actual_velocity());
    float rpmK = fabs(this->pMotorC->get_actual_velocity());
    float rpmL = fabs(this->pMotorD->get_actual_velocity());
    return 0.25 *(rpmI + rpmJ + rpmJ + rpmK);
}
