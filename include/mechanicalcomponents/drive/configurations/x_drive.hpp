#ifndef X_DRIVE_H
#define X_DRIVE_H
#include "../drive.hpp"
#include "TeriBull/TerriBull.hpp"
#include "pros/motors.hpp"

class X_Drive : public TerriBull::Drive {
    /**
     * @brief This Class has Four Motors! I have already defined one of them 
     * This class Implements TerriBull Drive to Drive to a position x,y!
     */
    
    private:
    pros::Motor pMotorA;
    /**
    This Class MUST Implement These Functions in order to properly implement the
    TerriBull::Drive Class! For more information on this:
        https://www.youtube.com/watch?v=77eueMbWI0Y -> Inheretence and Polymorphism

    This does not mean you cannot use Helper Functions to help you solve this implementation
     <in Fact, its encouraged to use as many as you need!>
    */
    void drive(float x, float y);
    void resultant_vector();
    void tare_encoders();

};

#endif