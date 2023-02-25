#ifndef __PROS_TERRIBULL_API__
#define __PROS_TERRIBULL_API__
#include "../api.h"

/*
Motors NEED to be created here in order to be able to use them in the funcitons.
*/

extern pros::Motor m1;
extern pros::Motor m2;
extern pros::Motor m3;
extern pros::Motor m4;

void x_drive_motors_set_voltages(int v1, int v2, int v3, int v4);

#endif