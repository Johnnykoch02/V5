#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "../TeriBull/TerriBull.hpp"
#include "../TerriBull/lib/Tasking/Task.hpp"
#include <list>

/**
 * @brief Methods:
void insert(index, data)	This function inserts a new item before the position the iterator points.
void push_back(data)	This functions add a new item at the list’s end.
void push_front(data)	It adds a new item at the list’s front.
data* pop_front()	It deletes the list’s first item.
size_t size()	This function determines the number of list elements.
data* front()	To determines the list’s first items.
data* back()	To determines the list’s last item.
reverse()	It reverses the list items.
merge()	It merges two sorted lists.
 * 
 */

class TerriBull::TaskManager {
private:
    TerriBull::TaskList *currentTaskSet;
    TerriBull::PriorityQueue<TerriBull::TaskList> tasks;
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
    void run() {
        if (this->currentTaskSet != nullptr)
        { /* Update and chek our current task*/
        int finishedTasks = 0, totalTasks = 0;
            for ( auto tsk = this->currentTaskSet->begin(); tsk != this->currentTaskSet->end(); ++tsk ) {
                tsk->update();
                            
                if (tsk->finishedFlag == true)
                { /* Task is complete */
                    delete this->currentTaskSet;
                    this->currentTaskSet = nullptr;
                }
            }
    
        }
        else if (tasks.isEmpty() == false)
        { /* Obtain the next task*/
            this->currentTaskSet = this->tasks.deque();
            for ( auto tsk = this->currentTaskSet->begin(); tsk != this->currentTaskSet->end(); ++tsk ) 
                tsk->init();
        }
        else
        {/* Do Nothing?*/
        // pros::lcd::print(3, "All Tasks Completed.");
    
        }
    }
    TerriBull::Task *current_task;
    TerriBull::PriorityQueue<TerriBull::Task> tasks;
public:
    TaskManager();
    void ClearAllTasks();
    void addTask(TerriBull::Task *task);
    void run();


};
#endif
