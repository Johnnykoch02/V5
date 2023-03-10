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

typedef enum TaskTypes {DRIVE, ROLLER, INTAKE, SHOOTER, TIME, INTERNAL_VARIABLES} TaskTypes;

 class TerriBull::Task {
    
   private:
   TaskTypes pTaskType;

   protected:
   TerriBull::MechanicalSystem* system;

   public:
   bool finishedFlag;
   bool terminated;
   // virtual Task(Task *task) = 0;
   // ~Task();
   Task(TaskTypes type, TerriBull::MechanicalSystem* system) :pTaskType(type), system(system), terminated(false) {}
   virtual void init() = 0;
   virtual void update(float delta) = 0;
   virtual void terminate() = 0;
 };


#endif
