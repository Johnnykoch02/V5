/**
 * @file TaskManager.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Manager for performing Task Sets. 
    Tasking Set Components:
        -   Drive
        -   Roller
        -   Shooter
        -   Intake
    
 * @version 0.1
 * @date 2022-10-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "../../TerriBull/TerriBull.hpp"
#include "../../TerriBull/lib/Tasking/Task.hpp"
#include <list>
// #include "../../TerriBull/lib/Expressions/Expression.hpp"

class TerriBull::TaskManager {
private:
    std::list<TerriBull::TaskList*> tasks;
    TerriBull::TaskList* currentTaskSet = nullptr;
    int tasksCompleted = 0;
    // std::vector<Expression*> Expressions;
public:
    TaskManager() = default;

    ~TaskManager() {
        ClearAllTasks();
    }

    void ClearAllTasks() {
        if (currentTaskSet != nullptr) {
            for (auto tsk : *currentTaskSet) {
                tsk->terminate();
                delete tsk;
            }
            delete currentTaskSet;
            currentTaskSet = nullptr;
        }
        for (auto taskList : tasks) {
            for (auto tsk : *taskList) {
                tsk->terminate();
                delete tsk;
            }
            delete taskList;
        }
        tasks.clear();
    }

    TerriBull::TaskList* InterruptCurrentTask() {
        TerriBull::TaskList* tasks = currentTaskSet;
        currentTaskSet = nullptr;
        if (tasks != nullptr) {
            for (auto tsk : *tasks) {
                tsk->terminate();
            }
        }
        return tasks;
    }

    void addTaskSet(TerriBull::TaskList* tasks) {
        this->tasks.push_back(tasks);
    }

    void Init() {}

    void run(float delta) {
        if (currentTaskSet != nullptr) { /* Update and check our current task */
            pros::lcd::set_text(7, "Task Number: " + std::to_string(tasksCompleted + 1));
            int finishedTasks = 0, totalTasks = 0;
            for (auto tsk : *currentTaskSet) {
                totalTasks++;
                if (tsk->finishedFlag != true) {
                    tsk->update(delta);
                } else {
                    finishedTasks++; /* Task is complete */
                    if (!tsk->terminated) {
                        tsk->terminate();
                    }
                }
            }
            if (totalTasks == finishedTasks) {
                for (auto tsk : *currentTaskSet) {
                    delete tsk;
                }
                delete currentTaskSet;
                currentTaskSet = nullptr;
                tasksCompleted++;
            }
        } else if (!tasks.empty()) { /* Obtain the next task */
            currentTaskSet = tasks.front();
            tasks.pop_front();
            for (auto tsk : *currentTaskSet) {
                tsk->init();
            }
        } else { /* Do Nothing? */
            pros::lcd::set_text(7, "All Tasks Completed.");
        }

        /* TODO: Optimize the way Expressions are updated such that the Expressions being updated are only those that are relevant to the Current Task */
        // for (auto expression : Expressions) {
        //     expression->updateTotal();
        // }
    }
};
#endif
