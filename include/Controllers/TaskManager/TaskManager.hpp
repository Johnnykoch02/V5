#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "../TeriBull/TerriBull.hpp"



class TerriBull::TaskManager {
public:
    TerriBull::Task *current_task;
    PriorityQueue<TerriBull::Task> tasks;
    TaskManager();
    void ClearAllTasks();
    void addTask(TerriBull::Task *task);
    void run();


};
#endif
