/**
 * @file SerialController.hpp
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

class TerriBull::SerialController {
    public:
    char buffer[500];

    SerialController();
    void update();
    void readBuffer();
    void processDataFromBuffer();
    void sendData(::std::string data);
};


#endif