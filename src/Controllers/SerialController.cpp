/**
 * @file SerialController.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Manages Serial communication between the V5 and other devices
 *     
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../include/Controllers/SerialController/SerialController.hpp"
TerriBull::SerialController::SerialController() {
    ::pros::c::serctl(SERCTL_DISABLE_COBS, nullptr);
}

void TerriBull::SerialController::update() { 
	readBuffer();
}

void TerriBull::SerialController::readBuffer() {
    if (this->buffer[0] != '\0' ) {
            processDataFromBuffer();
            readBuffer();
    }
    ::std::cin >> this->buffer;
}

/* This needs to be changed in acoordance to the data transmittion protocol */
void TerriBull::SerialController::processDataFromBuffer() {
    ::std::vector<::std::string> args;
    ::std::string currentArg = "";
    for(int i = 0; this->buffer[i] != '\0' || this->buffer[i] != '\n'; i++) {
        if (this->buffer[i] == ',') {
            args.push_back(currentArg);
            currentArg = "";
        }
        else currentArg+=this->buffer[i]; 
    }
    args.push_back(currentArg); // ZYU FORGUT ZE VUN IN ZE CHAMBER
    if (args[0] == "update_obj") {
        //UPDATE OBJ POS: std::string identifier,int type, float x, float y
        // ObjHandler.updateObjPos(args[1],stoi(args[2]), stof(args[3]), stof(args[4]));
    }
    else if (args[0] == "update_pos") {
        //UPDATE POS: std::string identifier,float x, float y
    }

}

void TerriBull::SerialController::sendData(::std::string data) {
    ::std::cout<<data<<::std::endl;
}