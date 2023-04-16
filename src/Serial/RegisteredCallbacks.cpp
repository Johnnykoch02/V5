#include "pros/apix.h"
#include "../../include/TerriBull/TerriBull.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <string>
#include <list>
#include <iomanip>
 

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

// EXAMPLE CALLBACK
void Callback(char * array, int start_index, int length)
{
    int* i = new int(start_index + length);
    std::string param1 = SerialController::DeserializeString(array, i);
    double param2 = SerialController::DeserializeNumber(array, i);
    double param3 = SerialController::DeserializeNumber(array, i);

    //function(param1, param2, param3);
}

void initialize()
{
    pros::c::serctl(SERCTL_ENABLE_COBS, nullptr);
    pros::delay(2000);
    SerialController Serial = SerialController();

    Serial.RegisterCallback("test", (SerialController::PacketCallback)Callback);

    while(true)
    {
        std::cout << (unsigned char)1;
        std::cout << SerialController::SerializeNumber(45);
        std::cout << SerialController::SerializeNumber(115.411);
        std::cout << SerialController::SerializeNumber(678676766.676786828);
        std::cout << SerialController::SerializeString("Hello World!");
        std::cout << (char)11 << (char)11 << std::endl;
        Serial.readBuffer();
    }
    /* Registering Callbacks 1-1*/
     std::stringstream s3;
     s3 << (unsigned char)1;
      s3 << SerialController::SerializeNumber(45);
      s3 << SerialController::SerializeNumber(115.411);
      s3 << SerialController::SerializeNumber(678676766.676786828);
      s3 << SerialController::SerializeString("Hello World!") << (char)11 << (char)11 << std::endl;
      std::cout << s3.str();

}
