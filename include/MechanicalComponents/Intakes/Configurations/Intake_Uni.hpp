/**
 * @file Intake_Uni.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Intake mechanism for Robots to take in Game Objects that utilizes a single motor.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
*/
#ifndef INTAKE_UNI_H
#define INTAKE_UNI_H

#include "../intake.hpp"
#include "../../pros/apix.h" 


class Intake_Uni : public TerriBull::Intake {
    protected:
    pros::Motor* pMotorI;

    public:
    Intake_Uni(int i, bool iReverse, int _maxSpeed, int gearSet) : TerriBull::Intake(_maxSpeed, gearSet) {
        this->pType = "Intake-Uni";
        this->pMotorI = new pros::Motor(i, (pros::motor_gearset_e) this->gearSet, iReverse);
        this->pMotorI->set_encoder_units(pros::E_MOTOR_ENCODER_COUNTS);
        this->pMotorRefs = (pros::Motor**) malloc(sizeof(pros::Motor*)*1);
        this->pMotorRefs[0] = pMotorI;
        this->motorRefs = new MechanicalComponent::MotorRefs {
        this->pType, this->pMotorRefs, 1
      };
    }

    int TurnOn(float dir); // button is held
    float getRPM() const;
    int TurnOff();
};     

#endif // INTAKE_H
