/**
 * @file SerialController.cpp
 * @author John Koch jkoch21@usf.edu, Bill Gate <Email Redacted>
 * @brief Manages Serial communication between the V5 and other devices
 *     
 * @version 0.1
 * @date 2023-03-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../include/Controllers/SerialController/SerialController.hpp"
TerriBull::SerialController::SerialController()
{
    ::pros::c::serctl(SERCTL_DISABLE_COBS, nullptr);
}

void TerriBull::SerialController::update()
{ 
	this->readBuffer();
}

bool TerriBull::SerialController::compareBuffer(vector<char> buffer1, int start, int end, char* buffer2) {
    for(int i = start; i < end; i++)
    {
        if (buffer1[i] != buffer2[i-start])
        {
            return false;
        }
    }
    return true;
}

void TerriBull::SerialController::readBuffer()
{
    std::string input;
    ::std::cin >> input;
    double last_length = 0;
    for (char _int : input)
    {
        int packet_length = sizeof(__next_packet);
        int lchar = __next_packet[packet_length-1];
        if (packet_length - 1 < lchar)
        {
            if (packet_length >= __header_length){
                __next_packet.push_back(_int);
                /* Checks to see if Transmition is terminated */
                if (this->compareBuffer(__next_packet, packet_length-__footer_length, packet_length, __end_of_transmission))
                {
                    DeserializePacket();
                    __next_packet.clear();
                    __next_packet.push_back(1);
                    continue;
                }
            }
            else if (_int == __packet_header[packet_length-1])
            {
                __next_packet.push_back(_int);
                continue;
            }
        }
        __next_packet.clear();
        __next_packet.push_back(_int);
    }
}

// 0 and 10 unusable
// 50-200 data types
// 11-255 data length - 10
// 11-255 integer - 11 
// 7 for decimal
// 11 end of transmission
// [start][function_id][data_type][data_length][data][end]
std::string TerriBull::SerialController::SerializeNumber( double f )
{
    std::ostringstream stream;
    std::string ff = "";

    if (f == (int)f)
        ff = std::to_string((int)f);
    else
        ff = std::to_string(f);

    for(int c = 0; c < ff.length(); c++)
    {
        unsigned char chr = ff[c];
        
        if (chr >= 48 and chr <= 57)
        {
            if (c+1 < ff.length() && ff[c+1] >= 48 && ff[c+1] <= 57)
            {
                unsigned char nchar = ((chr - 48) * 10) + (ff[c+1] - 37);
                stream << (unsigned char)nchar;
                c+=1;
            }
            else
            {
                stream << (unsigned char)(chr - 37);
            }
        }
        else if (chr == 46)
        {
            stream << (unsigned char)7;
        }
    }
    std::string ret = stream.str();
    ret = (char)(ret.length() + 10) + ret;
    ret = (char)1 + ret;
    return ret;
}

double TerriBull::SerialController::DeserializeNumber( char *array, int *si )
{
    int length = array[*si] - 10;
    double number = 0;
    double decimal = 0;
    int decimal_length = 0;
    bool switch_to_decimal = false;
    for(int i = *si+1; i < *si+length; i++)
    {
        if (!switch_to_decimal)
        {
            if ((int)array[i] != 7)
            {
                number *= 10;
                number += (int)(array[i] - 11);
            }
            else
            {
                switch_to_decimal = !switch_to_decimal;
            }
        }
        else
        {
            if ((int)array[i] != 7)
            {
                decimal_length++;
                decimal *= 10;
                decimal += (int)(array[i] - 11);
            }
            else
            {
                switch_to_decimal = !switch_to_decimal;
            }
        }
    }
    int divisor = pow(10, decimal_length);
    number += decimal / divisor;
    (*si)+=length+1;
    return number;
}

char* LongerLength( int length )
{
    char first_byte = (length & 0xFF00) >> 8;
    char second_byte = length & 0xFF;
    return new char[2] { first_byte, second_byte };
}

// MAX length 65,535 - 10
std::string TerriBull::SerialController::SerializeString( std::string s )
{
    return SerializeString(s.c_str());
}

std::string TerriBull::SerialController::SerializeString( const char *s )
{
    std::ostringstream stream;
    stream << (char)2;
    char* string_length = LongerLength(sizeof(s)+10);
    stream << (char)string_length[0];
    stream << (char)string_length[1];
    stream << s;
    return stream.str();
}

std::string TerriBull::SerialController::DeserializeString( char *array, int *si )
{
    std::ostringstream stream;
    int length = (array[*si] << 8) + array[(*si)+1];
    (*si)+=2;
    for (int i = *si; i < *si+length; i++){
        stream << array[i];
    }
    (*si)+=length;
    return stream.str();
}

void TerriBull::SerialController::ExchangeTags()
{

}

int TerriBull::SerialController::RegisterCallback(char *tag_name, PacketCallback callback)
{
    bool found = (std::find(CallTags.begin(), CallTags.end(), *tag_name) != CallTags.end());
    if (!found)
    {
        CallTags.push_back((*tag_name));
        Callbacks.push_back(callback);
    }
    else
    {
        throw new exception;
    }
    return 0;
}

void TerriBull::SerialController::DeserializePacket()
{
    PacketCallback __function = Callbacks[__next_packet[__header_length]];
    int pSize = sizeof(__next_packet);
    int end = pSize - __footer_length;
    char* buffer[end];
    memcpy( &buffer[0], &__next_packet[__header_length], end );
    __function((*buffer), 0, end);
}


void TerriBull::SerialController::sendData(::std::string data) {
    ::std::cout<<data<<::std::endl;
}

