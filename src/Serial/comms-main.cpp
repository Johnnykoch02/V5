#include "main.h"
#include "pros/apix.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <string>
#include <list>
 

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

// 0 and 10 unusable
// 50-200 data types
// 11-255 data length - 10
// 11-255 integer - 11 
// 7 for decimal
// 11 end of transmission
// [start][function_id][data_type][data_length][data][end]
std::string SerializeNumber( double f )
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
double DeserializeNumber( char *array, int *si )
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
std::string SerializeString( std::string s )
{
    return SerializeString(s.c_str());
}
std::string SerializeString( const char *s )
{
    std::ostringstream stream;
    stream << (char)2;
    char* string_length = LongerLength(sizeof(s)+10);
    stream << (char)string_length[0];
    stream << (char)string_length[1];
    stream << s;
    return stream.str();
}
std::string DeserializeString( char *array, int *si )
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

typedef int (*PacketCallback) (char * array, int start_index, int length);
std::list<PacketCallback> Callbacks;

void Callback(char * array, int start_index, int length)
{
    int* i = new int(start_index + length);
    std::string param1 = DeserializeString(array, i);
    double param2 = DeserializeNumber(array, i);
    double param3 = DeserializeNumber(array, i);

    //function(param1, param2, param3);
}

void ExchangeTags()
{

}

int RegisterCallback(char *tag_name, PacketCallback callback)
{
    
}

void DeserializePacket()
{

}

std::ostringstream __next_packet;
char __packet_header[] { (char)115, (char)111, (char)117, (char)116 };
char __end_of_transmission[] { (char)11, (char)11, (char)10, (char)0 };
int header_length = sizeof(__packet_header) + 1;
int footer_length = sizeof(__end_of_transmission);

void ReadPackets()
{
    double last_length = 0;
    std::string input;
    std::cin >> input;
    for (char _int : input)
    {
        int packet_length = sizeof(__next_packet);
        int lchar = __next_packet.rdbuf()->sgetc();
        if (packet_length - 1 < lchar)
        {
            if (packet_length >= header_length){
                __next_packet << _int;
                strcmp();
                if (__next_packet[-footer_length:] == __end_of_transmission)
                {
                    __ParsePacket(__next_packet);
                    __next_packet.str("");
                    __next_packet.clear();
                    __next_packet << (char)1;
                    continue;
                }
            }
            else if (_int == __packet_header[packet_length-1])
            {
                __next_packet.append(_int);
                continue;
            }
        }
        __next_packet.str("");
        __next_packet.clear();
        __next_packet << (char)_int;
    }
}

void initialize()
{
    pros::c::serctl(SERCTL_ENABLE_COBS, nullptr);
    pros::delay(2000);

    while(true)
    {
        std::cout << (unsigned char)1;
        std::cout << SerializeNumber(45);
        std::cout << SerializeNumber(115.411);
        std::cout << SerializeNumber(678676766.676786828);
        std::cout << SerializeString("Hello World!");
        std::cout << (char)11 << (char)11 << std::endl;
    }
}