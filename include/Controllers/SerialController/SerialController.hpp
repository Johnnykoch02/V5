/**
 * @file SerialController.hpp
 * @author John Koch jkoch21@usf.edu, Bill Gate <Email Redacted>
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
#include <queue>
#include <string>
#include <atomic>

class TerriBull::SerialController {
    public:
    typedef void (*PacketCallback) (TerriBull::RoboController* robot, char * array, int start_index, int length);

    typedef struct {
        PacketCallback callback;
        std::string friendly_name;
        int jetson_id;
    } CallbackItem;

    typedef struct {
        CallbackItem* callbackItem;
        float sumTime;
        float frequency;
    } ScheduledCallback;

    private:
    vector<char> __next_packet;
    char __packet_header[4] { (char)115, (char)111, (char)117, (char)116 };
    char __end_of_transmission[4] { (char)0, (char)0, (char)10, (char)10 };
    int __header_length = sizeof(__packet_header) + 1;
    int __footer_length = sizeof(__end_of_transmission);
    int __packet_index_offset = 15;
    bool isCollectingTags, tagExchange;
    map<int, CallbackItem*> Callbacks;
    vector<ScheduledCallback*> ScheduledCallbacks;
    TerriBull::RoboController* motherSys;

    std::string input_buffer;
    volatile bool buffer_has_data = false;

    bool CompareBuffer(vector<char> buffer1, int start, int end, char* buffer2);    

    public:
    struct UpdateArgs {
        volatile bool* buffer_update;
        std::string* buffer;
    };


    static void read_input_task(void* ignore)
    {
        pros::lcd::print(4, "I STARTED EATING POOP");
        SerialController::UpdateArgs* args = static_cast<SerialController::UpdateArgs*>(ignore);
        while (true) 
        {
            if (!*(args->buffer_update))
            {
                std::cin >> *(args->buffer);
                pros::lcd::print(5, args->buffer->c_str());
                *(args->buffer_update) = true;
                pros::delay(10);
            }
        }
    }
    SerialController(TerriBull::RoboController* _motherSys);

    static std::string SerializeNumber( double f );
    static double DeserializeNumber( char *array, int *si );
    static std::string SerializeString( std::string s );
    static std::string SerializeString( const char *s );
    static std::string DeserializeString( char *array, int *si );
    void ExchangeTags();
    int RegisterCallback(std::string tag_name, PacketCallback callback);
    void DeserializePacket();
    void Update(float delta);
    void ReadBuffer();
    void processDataFromBuffer();
    void SendData(::std::string data);
    void updateExchangeTags();
    bool isInitialized();
    void ProcessTagExchange(char * array, int start_index, int length);
    int GetCallbackIndex(std::string tag_name);
};


#endif
