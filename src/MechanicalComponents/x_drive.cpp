#include "../../include/MechanicalComponents/Drive/configurations/x_drive.hpp"
//: TerriBull::Drive(), pA(portA), pB(portB), pC(portC), pD(portD) 

X_Drive::X_Drive(int portA, int portB, int portC, int portD) {

    // this->setPID(0.5, 0.2, 0.3);
    // pMotorA.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // pMotorB.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // pMotorC.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // pMotorD.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

X_Drive::~X_Drive() {
    // delete pMotorA;
    // delete pMotorB;
    // delete pMotorC;

    // delete pMotorD;
}

float* X_Drive::drive(TerriBull::Vector2 pos) {
    return nullptr;
}

void X_Drive::change_orientation(float theta) {

}

void X_Drive::setVoltage(float* voltages, float lt, float lb, float rt, float rb)  {

}

void X_Drive::reset() {
}
