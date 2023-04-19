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

void AidanJoeShmo::Update(float delta) {
    /* Drive */
    bool drive_engaged = false;
    int yInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_LEFT_Y);
    if (abs(yInput) < deadzone) yInput = 0;
    if (yInput != 0) {
      float voltages[] = {0, 0, 0, 0, 0, 0};
      drive_engaged = true;
      yInput/=10.7;
      float V = yInput*fabs(yInput);
      voltages[0] = V; voltages[1] = V; voltages[2] = V; voltages[3] = V; voltages[4] = V; voltages[5] = V;
      this->roboController->getSystem()->getDrive()->setVoltage(voltages);
    }
    if (!drive_engaged) {
      int turnInput = controller.get_analog(::pros::E_CONTROLLER_ANALOG_RIGHT_X);
      if (abs(turnInput) < deadzone) turnInput = 0;
      if (turnInput != 0) {
        drive_engaged = true;

        turnInput /= 5;
        float angle = fmod((this->roboController->getSystem()->getAngle() - turnInput), 360);
        this->roboController->getSystem()->TurnToAngle(angle);
      }
    }
    if (!drive_engaged) this->roboController->getSystem()->ResetDrive();

    /* Intake */
    int in = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X);
    int out = controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y);
    if (in || out) {
      this->roboController->getSystem()->TurnOnIntake(in - out);
    } else this->roboController->getSystem()->TurnOffIntake();

    /* Roller */
    int l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    int r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    if (l1 || r1) {
      float currentPos = this->roboController->getSystem()->getRoller()->getPos();
      if(this->roboController->getSystem()->SpinRollerTo(currentPos + 50*(l1 - r1))) {
        pros::lcd::set_text(2, "Error with Roller");
      };
    } else this->roboController->getSystem()->ResetRoller();

    /* Shooter */
    int shoot = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    TerriBull::Shooter* shooter = this->roboController->getSystem()->getShooter();
    if (shoot || shooter->isToggled() ) {
      pros::lcd::set_text(2, "Shooting");
      if (shooter->isLoaded()) {
        shooter->Shoot(delta);
      }
      else {
        shooter->Load(delta, nullptr);
      }
    } 
    else shooter->reset();   
}