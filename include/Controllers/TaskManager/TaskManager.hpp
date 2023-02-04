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
    TerriBull::TaskList *currentTaskSet;
    TerriBull::PriorityQueue<TerriBull::TaskList> tasks;
    // ::std::vector<Expression*> Expressions;
public:
    TaskManager() {
        currentTaskSet = nullptr;
        this->tasks = TerriBull::PriorityQueue<TerriBull::TaskList>();
    }
    void ClearAllTasks() {
        this->tasks.deque_all();
        delete this->currentTaskSet;
        this->currentTaskSet = nullptr;
    }
    void addTaskSet(TaskList *tasks) {
        this->tasks.enqueue(tasks, 0);
    }

    void Init() {

    }

    void run() {
        if (this->currentTaskSet != nullptr)
        { /* Update and chek our current task*/
            int finishedTasks = 0, totalTasks = 0;
            for ( auto tsk : *(this->currentTaskSet)) {
                totalTasks++;
                tsk->update(0);
                            
                if (tsk->finishedFlag == true)
                { /* Task is complete */
                    finishedTasks++;
                }
            }
            if (totalTasks == finishedTasks) {
                delete this->currentTaskSet;
                this->currentTaskSet = nullptr;
            }
    
        }
        else if (tasks.isEmpty() == false)
        { /* Obtain the next task*/
            this->currentTaskSet = this->tasks.deque();
            for (  auto tsk : *(this->currentTaskSet) ) 
                tsk->init();
        }
        else
        {/* Do Nothing?*/
        // pros::lcd::print(3, "All Tasks Completed.");
    
        }
        /* TODO: Optimize the way Expressions are updated such that the Expressions being updated are only those that are realavent to the Current Task */
        // for (auto expression : this->Expressions) {
        //     expression->updateTotal();
        // }
    }

};
#endif
