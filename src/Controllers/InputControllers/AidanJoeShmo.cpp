/**
 * @file AidanJoeShmo.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Bull Two Config for Driver: Aidan aka. Joe Shmo.
 *
 * @version 0.1
 * @date 2023-02-28
 *
 * @copyright Copyright (c) 2022
 *
 */#include "../../../include/Controllers/InputController/Configurations/AidanJoeShmo.hpp"
void AidanJoeShmo::Init() {

}

void AidanJoeShmo::Update(int delta) {
    /* Drive Input */
    
    bool drive_engaged = false;
    int yInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int xInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_LEFT_X);
    if (abs(yInput) < deadzone) yInput = 0;
    if (abs(xInput)<deadzone) xInput = 0;
    if (yInput != 0 || xInput != 0) {
      drive_engaged = true;
      // pros::lcd::set_text(4,"Translation");
      Vector2* currentPos = this->roboController->getSystem()->getPosition();
      Vector2* goalPos = Vector2::cartesianToVector2(currentPos->x + xInput, currentPos->y + yInput);
      this->roboController->getSystem()->GoToPosition(goalPos->x, goalPos->y);
      delete goalPos;
    }

    int turnInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_RIGHT_X);
    if (abs(turnInput) < deadzone) turnInput = 0;
    if (turnInput != 0) {
      drive_engaged = true;
      pros::lcd::set_text(4,"Turn");
      turnInput /= 1.3;
      float angle = fmod((this->roboController->getSystem()->getAngle() - turnInput), 360);
      this->roboController->getSystem()->TurnToAngle(angle);
    }
    if (!drive_engaged) this->roboController->getSystem()->resetDrive();

    /* Intake */
    int in = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X);
    int out = controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y);
    if (in || out) {
      this->roboController->getSystem()->turnOnIntake(in - out);
    } else this->roboController->getSystem()->turnOffIntake();

}