/**
 * @file Task.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Tasking for the Robot requires an abstract class definition for it. 
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

   public:
   bool finishedFlag;
   // virtual Task(Task *task) = 0;
   // ~Task();
   virtual void init() = 0;
   virtual void update(float delta) = 0;
 };


#endif
#endif
 */
