/**
 * @file Task.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Tasking for the BullBot requires an abstract class definition for it. 
    Tasking Operations:
    
 * @version 0.1
 * @date 2022-10-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TASK_H
#define TASK_H

 #include "../../TerriBull.hpp"

typedef enum TaskTypes {DRIVE, ROLLER, SHOOTER, INTERNAL_VARIABLES} TaskTypes;

 class TerriBull::Task {
    
   private:
   TaskTypes pType;
   TerriBull::MechanicalSystem* system;

   public:
   bool finishedFlag;
   // virtual Task(Task *task) = 0;
   // ~Task();
   Task(TaskTypes type, TerriBull::MechanicalSystem* system) :pType(type), system(system) {}
   virtual void init() = 0;
   virtual void update(float delta) = 0;
 };


#endif
