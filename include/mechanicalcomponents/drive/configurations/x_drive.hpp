#ifndef X_DRIVE_H
#define X_DRIVE_H
#include "../drive.hpp"
#include "TeriBull/TerriBull.hpp"
#include "pros/motors.hpp"
#include <math.h>
#include <vector>

float rx,ry;

class X_Drive : public TerriBull::Drive {
    /**
     * @brief This Class has Four Motors! I have already defined one of them 
     * This class Implements TerriBull Drive to Drive to a position x,y!
     */
    
    private:
    pros::Motor pMotorA; // -> Top Left
    pros::Motor pMotorB; // -> Bottom Left
    pros::Motor pMotorC; // -> Top Right
    pros::Motor pMotorD; // -> Bottom Right

    // hello hi

    pros::Motor pMotorA;
    /**
    This Class MUST Implement These Functions in order to properly implement the
    TerriBull::Drive Class! For more information on this:
        https://www.youtube.com/watch?v=77eueMbWI0Y -> Inheretence and Polymorphism

    This does not mean you cannot use Helper Functions to help you solve this implementation
     <in Fact, its encouraged to use as many as you need!>
    */
    void drive(float x, float y) {
        

    }

    void resultant_vector()
    {
        //std::vector<float> vector3(5, 12);
    }

    void drive(float x, float y);
    void resultant_vector();
    void tare_encoders();

};

#endif

/*
struct Vector { float x, y, displacement; };

const Vector CARDINAL_VECTORS[9] = {
    Vector { 0.0, 1.0, VOLT_MAX }, Vector { 0.0, -1.0, VOLT_MAX },
    Vector { -1.0, 0.0, VOLT_MAX }, Vector { 1.0, 0.0, VOLT_MAX }, Vector { 0.0, 0.0, 0 },
    Vector { -1.0, 1.0, VOLT_MAX }, Vector { 1.0, 1.0, VOLT_MAX }, 
    Vector {BackWardLeft -1.0, -1.0, VOLT_MAX }, Vector { 1.0, -1.0, VOLT_MAX }
};

enum MotorCommands
{
    Forward,      Backward,
    Left,         Right,       Nil,
    ForwardLeft,  ForwardRight, 
    , BackwardRight
};

movefn(CARDINAL_VECTORS[MotorCommands::BackWards]);

*/
#endif