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
    typedef int (*PacketCallback) (char * array, int start_index, int length);

    private:
    vector<char> __next_packet;
    char __packet_header[4] { (char)115, (char)111, (char)117, (char)116 };
    char __end_of_transmission[4] { (char)11, (char)11, (char)10, (char)0 };
    int header_length = sizeof(__packet_header) + 1;
    int footer_length = sizeof(__end_of_transmission);
    bool compareBuffer(vector<char> buffer1, int start, int end, char* buffer2);
    std::list<PacketCallback> Callbacks;
    

    public:
    SerialController();
    static std::string SerializeNumber( double f );
    static double DeserializeNumber( char *array, int *si );
    static std::string SerializeString( std::string s );
    static std::string SerializeString( const char *s );
    static std::string DeserializeString( char *array, int *si );
    void ExchangeTags();
    int RegisterCallback(char *tag_name, PacketCallback callback);
    void DeserializePacket();
    void update();
    void readBuffer();
    void processDataFromBuffer();
    void sendData(::std::string data);
};


#endif