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
    else this->roboController->getSystem()->resetDrive();

    /* Roller */
    int l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    int r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    if (l1 || r1) {
      float currentPos = this->roboController->getSystem()->getRoller()->getPos();
      if(this->roboController->getSystem()->spinRollerTo(currentPos + 50*(l1 - r1))) {
        pros::lcd::set_text(2, "Error with Roller");
      };
    } else this->roboController->getSystem()->resetRoller();

}