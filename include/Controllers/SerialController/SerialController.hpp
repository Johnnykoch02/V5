/**
 * @file MechanicalSystem.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Manages Serial communication between the V5 and other devices
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <string>
#include "../../TerriBull/TerriBull.hpp"
#include "pros/apix.h" //added this in
#include <vector>
#include <iostream>

using namespace TerriBull;

class TerriBull::SerialController {
    public:
    char buffer[500];

    SerialController();
    void update();
    void readBuffer();
    void processDataFromBuffer();
    void sendData(::std::string data);
};


SerialController::SerialController() {
    pros::c::serctl(SERCTL_DISABLE_COBS, nullptr);
}

void SerialController::update() { 
	readBuffer();
}

void SerialController::readBuffer() {
    if (this->buffer[0] != '\0' ) {
            processDataFromBuffer();
            readBuffer();
    }
    ::std::cin >> this->buffer;
}

/* This needs to be changed in acoordance to the data transmittion protocol */
void SerialController::processDataFromBuffer() {
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

void sendData(std::string data) {
    std::cout<<data<<std::endl;
}
#endif