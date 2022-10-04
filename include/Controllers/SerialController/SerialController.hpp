#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <string>
#include "../TeriBull/TerriBull.hpp"

class SerialManager {
    public:
    char buffer[250];

    SerialManager();
    void update();
    void readBuffer();
    void processDataFromBuffer();
    void sendData(std::string data);
};


#endif