/**
 * @file DanTheMan.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Bull Two Config for Driver: Dan is in fact, the man.
 *
 * @version 0.1
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "../../../include/Controllers/InputController/Configurations/DanTheMan.hpp"
void DanTheMan::Init() {
    // this->roboController->getTaskManager()->ClearAllTasks();
}

void DanTheMan::Update(float delta) {
    /* Drive */
    bool drive_engaged = false;
    int xInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_LEFT_X);
    int yInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_LEFT_Y);
    if (abs(xInput) < deadzone) xInput = 0;
    if (abs(yInput) < deadzone) yInput = 0;
    if (xInput || yInput) {
      xInput /= 10;
      yInput /= 10;
      drive_engaged = true;
      Vector2* currentPos = this->roboController->getSystem()->getPosition();
      int angleMod = (yInput > 0) ? 1 : 0;
      Vector2* dPos = Vector2::cartesianToVector2( xInput, yInput );
      Vector2* goalPos = *currentPos + *dPos;
      // this->roboController->getSystem()->GoToPosition(*goalPos);
      delete goalPos;
      delete dPos;
    }

    int turnInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_RIGHT_X);
    if (abs(turnInput) < deadzone) turnInput = 0;
    if (turnInput != 0) {
      drive_engaged = true;
      
      turnInput /= 5;
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

    /* Shooter */
    // TerriBull::Shooter* shooter = this->roboController->getSystem()->getShooter();
    // if (shooter->isToggled()) {
    //   shooter->Shoot(delta);
    //   if (shooter->shotCompleted()) {
        // shooter->reset();
    //   }
    // } else {
    //   int shoot = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    //   if (shoot) {
        // shooter->Shoot(delta);
    //   }
    // }
}