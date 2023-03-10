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
#include <sstream>
#include <iomanip>
X_Drive::~X_Drive() {
    delete this->pMotorA;
    delete this->pMotorB;
    delete this->pMotorC;
    delete this->pMotorD;
}

void X_Drive::setVoltage(float* vals)  {
/* Less than some threshold */ 
    float lt = vals[0];float lb = vals[1];float rt = vals[2];float rb = vals[3];
    if (fabs(lt) > this->maxSpeed) lt = this->maxSpeed * fabs(lt) / lt;
    if (fabs(rt) > this->maxSpeed) rt = this->maxSpeed * fabs(rt) / rt;
    if (fabs(lb) > this->maxSpeed) lb = this->maxSpeed * fabs(lb) / lb;
    if (fabs(rb) > this->maxSpeed) rb = this->maxSpeed * fabs(rb) / rb;
    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(1);
     s3 << "pWRS: " << lt << " " << lb << " " << rt << " " << rb;
    pros::lcd::set_text(6,s3.str());
    this->pMotorA->move(lt);
    this->pMotorB->move(lb);
    this->pMotorC->move(rt);
    this->pMotorD->move(rb);

}

int X_Drive::drive(TerriBull::Vector2 pos, float delta) {
    /* Theta of desired Modified By our current Look Angle */
    float* vals = new float[4];
    Vector2* dP = (pos - *(this->pCurrentPos));
    double angle = fmod((RAD2DEG(dP->theta) + (*(this->pCurrentAngle)- 90)) , 360.0);
    angle = (angle < 0)? angle + 360.0 : angle;
    int x = int(round(angle/45)) % 8;
    int dir = 0;
    float pct = 0;
    
    /* Basic PID Equation */
    this->currentError = dP->r;
    // ::pros::lcd::set_text(6, "Drive Error: " + std::to_string(this->pCurrentPos->theta));
    this->sumError+=currentError;
    pct = kP*this->currentError + kI*this->sumError + kD*this->dError() / delta;
    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(1);
     s3 << "Err: "<< this->currentError <<  "|" << x <<" " << angle;
    pros::lcd::set_text(4,s3.str());

    /* X Drive Drives in all 8 directions, pick the direction an apply the right direction modifier */
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
        // pros::lcd::set_text(4,"DEFAULT");
          delete[] vals;
          return -1;
    }
    this->setVoltage(vals);
    delete[] vals;
    delete dP;
    return 0;
}

int X_Drive::change_orientation(float theta, float delta) {
  float* vals = new float[4];
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));
  ::pros::lcd::set_text(6, "Theta Error: " + std::to_string(this->currentError));
  this->sumError += this->currentError;
  float pwr = this->kPTheta * this->currentError + this->kI * this->sumError + this->kDTheta * this->dError();
  ::pros::lcd::set_text(7, "Drive Pct: " + std::to_string(pwr));
  vals[0] = -pwr * fabs(this->currentError)/this->currentError;
  vals[1] = 0; 
  vals[2] = 0;
  vals[3] = pwr * fabs(this->currentError)/this->currentError;
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

Vector2* X_Drive::resultant_vector() {
  // return Vector2::cartesianToVector2(0, 0);
    float l1 = this->pMotorA->get_position();
    float l2 = this->pMotorB->get_position();
    float r1 = this->pMotorC->get_position();
    float r2 = this->pMotorD->get_position();
    float left = ((l1 + r2) / 2) * this->wheelRadius * this->conversionFactor / ENCODER_UNIT[this->gearSet]; /* Assuming Radius of wheel is 5 */
    float right = ((l2 + r1) / 2) * this->wheelRadius * this->conversionFactor / ENCODER_UNIT[this->gearSet]; /* Assuming Radius of wheel is 5 */
    
    int leftDir = fabs(left) / left;
    int rightDir = fabs(right) / right;
    float leftAngle = (leftDir > 0) ? fmod((-45 + *(this->pCurrentAngle)), 360.0) : fmod((135 + *(this->pCurrentAngle)), 360.0);
    float rightAngle = (rightDir > 0)? fmod((45 + *(this->pCurrentAngle)), 360.0) : fmod((225 + *(this->pCurrentAngle)), 360.0);
    Vector2* vecLeft = Vector2::polarToVector2(fabs(left), DEG2RAD(leftAngle));
    Vector2* vecRight = Vector2::polarToVector2(fabs(right), DEG2RAD(rightAngle));
    Vector2* vecUnNormalized = *vecLeft + *vecRight;
    // Vector2* resultantVector = *vecUnNormalized * 0.5;
    
    /* Cleanup */
    this->tare_encoders();
    delete vecLeft;
    delete vecRight;
    return vecUnNormalized;    
}

