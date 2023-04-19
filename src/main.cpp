/**
 * @file main.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Main Target ran on V5
 *
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */


// namespace BULL2_XD {
	// pros::Motor pMotorA (1, pros::E_MOTOR_GEARSET_18, false);
	// pros::Motor pMotorB (2, pros::E_MOTOR_GEARSET_18, false);
	// pros::Motor pMotorC (3, pros::E_MOTOR_GEARSET_18, true);
	// pros::Motor pMotorD (4, pros::E_MOTOR_GEARSET_18, true);
// };



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
TerriBull::RoboController controlSys;


void on_center_button() {
	//controlSys.ClearTasks();
	// pros::delay(100);
	// controlSys.getSystem()->resetDrive();
}

void TestFunction( RoboController* c, char * array, int start_index, int length )
{
	int* i = new int(start_index);
	double number0 = SerialController::DeserializeNumber(array, i);
	double number1 = SerialController::DeserializeNumber(array, i);
	std::string string0 = SerialController::DeserializeString(array, i);

	char* cNumber0 = new char(number0 + 48);
	char* cNumber1 = new char(number1 + 48);
	pros::lcd::clear_line(0);
	pros::lcd::print(0, cNumber0);
	pros::lcd::clear_line(1);
	pros::lcd::print(1, cNumber1);
	pros::lcd::clear_line(2);
	pros::lcd::print(2, string0.c_str());
}

void initialize() {
	pros::lcd::initialize();
	// controlSys.Init();
	SerialController* serialController = new SerialController(nullptr);
	serialController->RegisterCallback("TestFunction", (SerialController::PacketCallback)TestFunction);
	pros::lcd::print(0, "Exchanging Tags...");
	serialController->ExchangeTags();
	pros::lcd::print(0, "Finished            ");
	while(true)
	{
		serialController->ReadBuffer();
		
		stringstream stream;
		stream << serialController->GetCallbackIndex("test_function");
		stream << SerialController::SerializeNumber(654);
		stream << SerialController::SerializeString("this is a super long string that we are sending over serial that i hope will just be over 255 characters to ensure strings are working properly");
		stream << SerialController::SerializeNumber(5);
		stream << SerialController::SerializeNumber(6568964);
		stream << SerialController::SerializeNumber(7896786);
		serialController->SendData(stream.str());
	}
	//pros::lcd::register_btn1_cb(on_center_button);
}


void autonomous() {
	//while (true) {
	//	controlSys.Run();
	//}
}



void opcontrol() {
	// controlSys.ClearTasks();
	//controlSys.getSystem()->setStartingPosition(0, 0);
	//while (true) {
	//	controlSys.Run();
    //}
}
