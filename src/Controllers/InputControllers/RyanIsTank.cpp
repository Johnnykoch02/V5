/**
 * @file RyanIsTank.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Bull Two Config for Driver: O' Captian my Captian, Nathaniel
 *
 * @version 0.1
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "../../../include/Controllers/InputController/Configurations/RyanIsTank.hpp"
void RyanIsTank::Init() {
    // this->roboController->getTaskManager()->ClearAllTasks();
}

void RyanIsTank::Update(float delta) {
    /* Drive */
    bool drive_engaged = false;
    int lInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    if (abs(lInput) < deadzone) lInput = 0;
    if (abs(rInput) < deadzone) rInput = 0;
    float voltages[] = {0, 0, 0, 0, 0, 0};
    if (lInput != 0) {
      drive_engaged = true;
      lInput/=10.7;
      float lV = lInput*fabs(lInput);
      voltages[0] = lV; voltages[1] = lV; voltages[2] = lV;
    }
    if (rInput != 0) {
      drive_engaged = true;
      rInput/=10.7;
      float rV = rInput*fabs(rInput);
      voltages[3] = rV; voltages[4] = rV; voltages[5] = rV;
    }

    if (drive_engaged) this->roboController->getSystem()->getDrive()->setVoltage(voltages);
    else this->roboController->getSystem()->ResetDrive();
    /* TODO: Change Constriction such that the motor Groups are checked for their Current first.*/
    /* Roller */
    int l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    int r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    if (l1 || r1) {
      this->roboController->getSystem()->UnConstrictMotorGroupCurrent(this->roboController->getSystem()->getRoller()->getMotorRefs());
      float currentPos = this->roboController->getSystem()->getRoller()->getPos();
      if(this->roboController->getSystem()->TurnOnRoller(1.5*(r1-l1))) {
        pros::lcd::set_text(2, "Error with Roller");
      }
    } 
    else {
      this->roboController->getSystem()->ResetRoller();
      this->roboController->getSystem()->TurnOffRoller();
      if (!this->roboController->getSystem()->isRollerToggled()) {
        this->roboController->getSystem()->ConstrictMotorGroupCurrent(this->roboController->getSystem()->getRoller()->getMotorRefs());
      }
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
       if (this->roboController->getSystem()->isIntakeToggled()) {
        this->roboController->getSystem()->TurnOffIntake();
        this->roboController->getSystem()->ConstrictMotorGroupCurrent(this->roboController->getSystem()->getIntake()->getMotorRefs());
      } else {
      this->roboController->getSystem()->UnConstrictMotorGroupCurrent(this->roboController->getSystem()->getIntake()->getMotorRefs());
        this->roboController->getSystem()->TurnOnIntake(-1);
        // this->roboController->getSystem()->getShooter()->Shoot(delta, nullptr);
    }
    }

    /* Shooter */ /*TODO: Change To Toggle */
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
      if (this->roboController->getSystem()->isShooterToggled()) {
        this->roboController->getSystem()->ResetShooter();
        this->roboController->getSystem()->ConstrictMotorGroupCurrent(this->roboController->getSystem()->getShooter()->getMotorRefs());
      } else {
      this->roboController->getSystem()->UnConstrictMotorGroupCurrent(this->roboController->getSystem()->getShooter()->getMotorRefs());
        this->roboController->getSystem()->TurnOnShooter();
        // this->roboController->getSystem()->getShooter()->Shoot(delta, nullptr);
      }
    }
}