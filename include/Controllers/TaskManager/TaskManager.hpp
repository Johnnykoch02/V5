#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "../TeriBull/TerriBull.hpp"



class TerriBull::TaskManager {
private:
    TerriBull::Task *current_task;
    TerriBull::PriorityQueue<TerriBull::Task> tasks;
public:
    TaskManager();
    void ClearAllTasks();
    void addTask(TerriBull::Task *task);
    void run();


};
#endif
