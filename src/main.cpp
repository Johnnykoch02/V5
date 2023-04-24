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

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

int kghkh = 1;
void TestFunction( RoboController* c, char *array, int start_index, int length )
{
	int i = start_index + 1;

	double number0 = SerialController::DeserializeNumber(array, &i);
	i++;
	double number1 = SerialController::DeserializeNumber(array, &i);
	i++;
	std::string string0 = SerialController::DeserializeString(array, &i);

	pros::lcd::set_text(0, to_string(kghkh++).c_str());
	pros::lcd::set_text(1, to_string(number0).c_str());
	pros::lcd::set_text(2, to_string(number1).c_str());
	
	int u = 3;
	std::string fmt;
	for (int p = 0; p < string0.length(); p++)
	{
		//if (u > 7) u = 2;
		fmt += string0[p];
		if (fmt.length() > 26)
		{
			trim(fmt);
			if (fmt.length() > 0)
				pros::lcd::set_text(u++, fmt.c_str());
			fmt.clear();
		}
	}
	trim(fmt);
	if (fmt.length() > 0)
		pros::lcd::set_text(u++, fmt.c_str());
}


void initialize() {
	pros::lcd::initialize();
	// controlSys.Init();
	SerialController* serialController = new SerialController(nullptr);
	serialController->RegisterCallback("TestFunction", (SerialController::PacketCallback)TestFunction);
	serialController->ExchangeTags();

	int sent = 1;
	while(true)
	{
		serialController->ReadBuffer();

		stringstream stream;
		stream << (unsigned char) (serialController->GetCallbackIndex("test_function"));
		stream << SerialController::SerializeNumber(654);
		stream << SerialController::SerializeString("this is a super long string that we are sending over serial that i hope will just be over 255 characters to ensure strings are working properly");
		stream << SerialController::SerializeNumber(1);
		stream << SerialController::SerializeNumber(715.4);
		stream << SerialController::SerializeNumber(789.6786);
		serialController->SendData(stream.str());
		pros::lcd::set_text(7, to_string(sent++).c_str());
		pros::delay(5);
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
