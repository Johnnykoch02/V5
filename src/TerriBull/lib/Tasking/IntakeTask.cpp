/**
 * @file IntakeTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the Intake system of the BullBot.
 *     
 * @version 0.1
 * @date 2023-03-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../../../include/TerriBull/lib/Tasking/Types/IntakeTask.hpp"
IntakeTask::IntakeTask(float dir, IntakeType intakeType, TerriBull::MechanicalSystem* _system) : Task(INTAKE, _system), intakeType(intakeType), dir(dir) {}

void IntakeTask::init() {}

void IntakeTask::update(float delta) {
    if (!this->finishedFlag) {
        switch (intakeType) {
            case ON:
                this->system->turnOnIntake(this->dir);
                this->finishedFlag = (this->system->getIntake()->isToggled()) && (this->system->getIntake()->getDirection() == this->dir);
                break;
            case OFF:
                this->system->turnOffIntake();
                this->finishedFlag = !(this->system->getIntake()->isToggled());
                break;
        }
    }
}

void IntakeTask::terminate() {

}