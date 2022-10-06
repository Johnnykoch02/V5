#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <string>
#include "../TeriBull/TerriBull.hpp"

class TerriBull::SerialController {
    public:
    char buffer[250];

    SerialController();
    void update();
    void readBuffer();
    void processDataFromBuffer();
    void sendData(::std::string data);
};


#endif