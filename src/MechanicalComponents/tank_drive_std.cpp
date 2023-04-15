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
#include <sstream>
#include <iomanip>
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
    /*TODO: Implement VoltageRegulator Class that Registers each of these voltages as a */
    if (fabs(vals[0]) > this->maxSpeed) vals[0] = this->maxSpeed * fabs(vals[0]) / vals[0];
    if (fabs(vals[3]) > this->maxSpeed) vals[3] = this->maxSpeed * fabs(vals[3]) / vals[3];
    if (fabs(vals[1]) > this->maxSpeed) vals[1] = this->maxSpeed * fabs(vals[1]) / vals[1];
    if (fabs(vals[4]) > this->maxSpeed) vals[4] = this->maxSpeed * fabs(vals[4]) / vals[4];
    if (fabs(vals[2]) > this->maxSpeed) vals[2] = this->maxSpeed * fabs(vals[2]) / vals[2];
    if (fabs(vals[5]) > this->maxSpeed) vals[5] = this->maxSpeed * fabs(vals[5]) / vals[5];
    if (this->pUseVoltageRegulator) vals = this->pVoltageRegulator->getRegulatedVoltages(vals);
    float lt = vals[0]; float lm = vals[1];float lb = vals[2]; float rt = vals[3]; float rm = vals[4]; float rb = vals[5];

    std::stringstream s3;
    s3 << std::fixed << ::std::setprecision(2);
    s3 << "Left: "<< lm << " Right: " << rm;
    pros::lcd::set_text(5, s3.str());
    this->pMotorA->move(lt);
    this->pMotorB->move(lm);
    this->pMotorC->move(lb);
    this->pMotorD->move(rt);
    this->pMotorE->move(rm);
    this->pMotorF->move(rb);
}

int Tank_Drive_Std::drive(TerriBull::Vector2 v_f, TerriBull::Vector2 v_i, float delta, bool reverse) {
    /**
     * @brief Equations: Applied Kinematics
     * @cite Control of Mobile Robots https://drive.google.com/file/d/1Aakq3O7vaGR-X-FKcNERPgBE4h5oWohU/view?usp=share_link
     * @author https://www.usf.edu/engineering/cse/people/weitzenfeld-alfredo.aspx
     */
    int angleMod = (reverse) ? 180 : 0;
    Vector2* dPos = v_f - *(this->pCurrentPos);
    Vector2* dPos_Unit = dPos->unit();
    Vector2* dPos_Scaled = (*dPos_Unit)*((0.5*this->wheelBase)+1.5);
    float currentAngle = DEG2RAD(fmod(*(this->pCurrentAngle) + angleMod, 360.0));
    float deltaAngle = DEG2RAD(GetDTheta(RAD2DEG(dPos->theta), RAD2DEG(currentAngle)));
    Vector2* ICC = Vector2::cartesianToVector2(dPos_Scaled->x-(0.5*this->wheelBase)*sin(currentAngle), dPos_Scaled->y+(0.5*this->wheelBase)*sin(currentAngle));
    float omega = deltaAngle / ICC->r;
    float vLeft = omega * (ICC->r - (this->wheelBase / 2));
    float vRight = omega * (ICC->r - (this->wheelBase / 2));
    this->currentError = dPos->r;
    float leftProportional = vLeft / vRight;
    float rightProportional = vRight / vLeft;
    float y_t = this->kP * this->currentError + this->kD * this->dError() / delta;
    /* We need to Scale our vLeft and vRight such that the Proportion is maintained but the power is relative to y_t */
    if (fabs(leftProportional) > fabs(rightProportional)) { /* Lets start the scale on the Left Side*/
        vLeft = std::clamp(vLeft*y_t, -this->maxSpeed, this->maxSpeed);
        vRight = vLeft * rightProportional;
    }
    else { /* Lets start the scale on the Right Side*/
        vRight = std::clamp(vRight*y_t, -this->maxSpeed, this->maxSpeed);
        vLeft = vRight * leftProportional;
    }
    float voltages[] = {vLeft, vLeft, vLeft, vRight, vRight, vRight};
    this->setVoltage(voltages);
    /* Clean Memory */
    delete dPos;
    delete dPos_Unit;
    delete ICC;
    return 0;   
}

int Tank_Drive_Std::change_orientation(float theta, float delta) {
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

void Tank_Drive_Std::maneuverAngle(float theta, float delta, float r, int errorMod) {
   float Kr = 0.028;
  float* vals = new float[6];
  this->currentError = GetDTheta(theta, *(this->pCurrentAngle));
  this->sumError += this->currentError;
  float pwr = r*Kr*(this->kPTheta * this->currentError + this->kDTheta * this->dError() / delta);
  float thirdPwr = -pwr*0.3;
  std::stringstream s3;
  s3 << std::fixed << ::std::setprecision(1);
  s3 << "Err: "<< this->currentError << " Pwr: " << pwr;
  pros::lcd::set_text(4,s3.str());
  float lPwr = 0 , rPwr = 0;
  if (errorMod > 0) {
    lPwr = MAX(0, pwr);
    rPwr = MIN(pwr, 0);
    lPwr = (lPwr == 0) ? thirdPwr : lPwr;
    rPwr = (rPwr == 0)? thirdPwr : rPwr;
  }
  else if (errorMod < 0) {
    lPwr = MIN(pwr, 0);
    rPwr = MAX(0, pwr);
    lPwr = (lPwr == 0)? thirdPwr : lPwr;
    rPwr = (rPwr == 0)? thirdPwr : rPwr;
  }
  vals[0] = lPwr;
  vals[1] = lPwr;
  vals[2] = lPwr;
  vals[3] = rPwr;
  vals[4] = rPwr;
  vals[5] = rPwr;
  this->setVoltage(vals);
  delete[] vals;
}

void Tank_Drive_Std::reset() {
    this->pMotorA->move(0);
    this->pMotorB->move(0);
    this->pMotorC->move(0);
    this->pMotorD->move(0);
    this->pMotorE->move(0);
    this->pMotorF->move(0);
    this->currentError = 0;
    this->sumError = 0;
    this->previousError = 0;
    this->pNeedsAngleCorrection = false;
    if (this->pUseVoltageRegulator) {
        this->pVoltageRegulator->ResetGradients();
    }
}

Vector2* Tank_Drive_Std::resultant_vector() {
    float l1 = this->pMotorA->get_position();
    float l2 = this->pMotorB->get_position();
    float l3 = this->pMotorC->get_position();
    float r1 = this->pMotorD->get_position();
    float r2 = this->pMotorE->get_position();
    float r3 = this->pMotorF->get_position();
    float left = ((l1 + l2 + l3) / 3) * 2 * PI * this->wheelRadius * this->conversionFactor / ENCODER_UNIT[this->gearSet]; /* Assuming Radius of wheel is 5 */
    float right = ((r1 + r2 + r3) / 3) * 2 * PI  * this->wheelRadius * this->conversionFactor / ENCODER_UNIT[this->gearSet]; /* Assuming Radius of wheel is 5 */
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

/// Old Drive Code using PID
///* Theta of desired Modified By our current Look Angle */
//     float* vals = new float[6];
//     float pct = 0;
//     Vector2* v_to_goal = (v_f - *(this->pCurrentPos));
//     Vector2* v_i_to_goal = (v_f - v_i);
//     Vector2* v_i_to_bot = (*(this->pCurrentPos) - v_i);
//     int8_t rev = (reverse)? -1 : 1;
//     /* Overshooting: Bot traveled further than distance, and the Vectors of displacement are somewhat in the same direction */
//     int8_t errorMod = (v_i_to_goal->r < v_i_to_bot->r && *(v_i_to_goal) * *( v_i_to_bot) > 0.5) ? 1 : -1;
//     // if (v_to_goal->r > errorMod) targetDirection = errorMod;
//     this->currentError = v_to_goal->r * errorMod; /* First Part: Absolute Displacement, Second Part: Positive or Negative */
//     this->sumError+=currentError;
//     this->sumError = std::clamp(this->sumError, -200.0f, 200.0f);
//     std::stringstream s3;
//     s3 << std::fixed << ::std::setprecision(1);
//     s3 << "Err: "<< this->currentError << " Mod: " << errorMod << "|" << RAD2DEG(v_to_goal->theta) << " " << v_to_goal->r;
//     pros::lcd::set_text(4,s3.str());
//     /* Basic PID Equation */
//     pct = kP*currentError + kI*this->sumError + kD*this->dError() / delta;
//     if (fabs(pct) >  this->maxSpeed) {/* Clmp Pwr to this->maxSpeed */
//         pct = this->maxSpeed * fabs(pct) / pct;
//     }
//     float pL = pct*rev;
//     float pR = pct*rev;
//     int angleMod = (rev > 0) ? 0 : 180;
//     float offTrack = GetDTheta(RAD2DEG(v_to_goal->theta),  fmod(*(this->pCurrentAngle) + angleMod, 360))*v_to_goal->r;
//     if (fabs(v_to_goal->r) > 6.0 && offTrack > 65 ) {
//         this->pNeedsAngleCorrection = true;
//         float resetVoltages[] = {0, 0, 0, 0, 0, 0};
//         this->setVoltage(resetVoltages);
//         delete v_to_goal;
//         delete v_i_to_goal;
//         delete v_i_to_bot;
//         return 0;
//     }
//     else if (fabs(v_to_goal->r) > 3) {
//         int dir = fabs(offTrack)/offTrack;
//         pL *= 0.78;
//         pR *= 0.78;
//         pL += MIN(fabs(this->kPThetaTranslation*offTrack), fabs(0.1* pct)) * dir * errorMod * rev;
//         pR -= MIN(fabs(this->kPThetaTranslation*offTrack), fabs(0.1* pct)) * dir * errorMod * rev;
//     }
//     // else if (fabs(v_to_goal->r) > 0.5 && fabs(offTrack) > 15) {
//     //     this->pNeedsAngleCorrection = true;
//     //     float resetVoltages[] = {0, 0, 0, 0, 0, 0};
//     //     this->setVoltage(resetVoltages);
//     //     delete v_to_goal;
//     //     delete v_i_to_goal;
//     //     delete v_i_to_bot;
//     //     return 0;
//     // }
//     // else if (fabs(dP->r) > 0.5 && offTrack > 10) {
//     //     this->maneuverAngle(fmod(RAD2DEG(dP->theta) + angleMod, 360), delta, dP->r, errorMod);
//     //     delete[] vals;
//     //     delete dP;
//     //     return 0;
//     // }
//     vals[0] = pL;
//     vals[1] = pL;
//     vals[2] = pL;
    
//     vals[3] = pR;
//     vals[4] = pR;
//     vals[5] = pR;
//     this->setVoltage(vals);
//     delete[] vals;
//     delete v_to_goal;
//     delete v_i_to_goal;
//     delete v_i_to_bot;   
//     return 0;
// }