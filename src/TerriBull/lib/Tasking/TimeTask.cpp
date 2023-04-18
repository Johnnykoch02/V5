/**
 * @file TimeTask.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Task Targetting the drive system of the BullBot For Shooting Disks.
 *     
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../../../include/TerriBull/lib/Tasking/Types/TimeTask.hpp"
TimeTask::TimeTask(float goalTime, TerriBull::MechanicalSystem* _system) : Task(TIME, _system), goalTime(goalTime) {}

TimeTask::~TimeTask() {}

void TimeTask::init() {
    this->finishedFlag = false;
}

void TimeTask::update(float delta) {
    // pros::lcd::set_text(4,);
    if (!this->finishedFlag) {
        this->sumTime+=delta;
        this->finishedFlag = this->sumTime > this->goalTime;

    }
}

void TimeTask::terminate() {
    pros::lcd::set_text(4,"LEAVING TIME TASK");
    this->terminated = true;
}