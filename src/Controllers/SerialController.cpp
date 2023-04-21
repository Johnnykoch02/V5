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
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "../../include/Controllers/SerialController/SerialController.hpp"

std::string TerriBull::SerialController::input_buffer;
pros::Mutex TerriBull::SerialController::input_mutex;

TerriBull::SerialController::SerialController(TerriBull::RoboController *_motherSys) : motherSys(_motherSys), tagExchange(false), isCollectingTags(false)
{
    ::pros::c::serctl(SERCTL_DISABLE_COBS, nullptr);
    pros::Task input_task(SerialController::read_input_task);
}

void TerriBull::SerialController::Update(float delta)
{
    this->ReadBuffer();
    for (auto it = this->ScheduledCallbacks.begin(); it != this->ScheduledCallbacks.end(); ++it)
    {
        ScheduledCallback *callback = *it;
        callback->sumTime += delta;
        if (callback->sumTime >= callback->frequency)
        {
            callback->callbackItem->callback(this->motherSys, nullptr, 0, 0);
            callback->sumTime = 0;
        }
    }
}

bool TerriBull::SerialController::isInitialized()
{
    return this->tagExchange;
}

bool TerriBull::SerialController::ReadBuffer()
{
    std::string input;
    std::unique_lock<pros::Mutex> lock(TerriBull::SerialController::input_mutex);
    if (!TerriBull::SerialController::input_buffer.empty())
    {
        input = TerriBull::SerialController::input_buffer;
        TerriBull::SerialController::input_buffer.clear();
    }
    lock.unlock();

    if (input.size() == 0)
        return false;

    // return;

    for (char _int : input)
    {
        int packet_length = __next_packet.size();
        if (packet_length >= __header_length)
        {
            __next_packet.push_back(_int);
            /* Checks to see if Transmition is terminated */
            if (std::equal(__next_packet.end() - __footer_length, __next_packet.end(), std::begin(__end_of_transmission),
                           [](int a, char b)
                           { return a == (int)b; }))
            {
                this->DeserializePacket();
                __next_packet.clear();
            }
            continue;
        }
        else if (_int == __packet_header[packet_length])
        {
            __next_packet.push_back(_int);
            continue;
        }
        __next_packet.clear();
    }
    
    return true;
}

// 0 and 10 unusable
// 50-200 data types
// 11-255 data length - 10
// 11-255 integer - 11
// 7 for decimal
// 11 end of transmission
// [start][function_id][data_type][data_length][data][end]
std::string TerriBull::SerialController::SerializeNumber(double f)
{
    std::ostringstream stream;
    std::string ff = "";

    if (f == (int)f)
        ff = std::to_string((int)f);
    else
        ff = std::to_string(f);

    for (int c = 0; c < ff.length(); c++)
    {
        unsigned char chr = ff[c];

        if (chr >= 48 and chr <= 57)
        {
            if (c + 1 < ff.length() && ff[c + 1] >= 48 && ff[c + 1] <= 57)
            {
                unsigned char nchar = ((chr - 48) * 10) + (ff[c + 1] - 37);
                stream << (unsigned char)nchar;
                c += 1;
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

double TerriBull::SerialController::DeserializeNumber(char *array, int *si)
{
    int length = array[*si] - 10;
    (*si)++;
    double number = 0;
    double decimal = 0;
    int decimal_length = 0;
    bool switch_to_decimal = false;
    for (int i = *si; i < *si + length; i++)
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
    (*si) += length;

    return number;
}

char *LongerLength(int length)
{
    char first_byte = (length & 0xFF00) >> 8;
    char second_byte = length & 0xFF;
    return new char[2]{first_byte, second_byte};
}

// MAX length 65,535 - 10
std::string TerriBull::SerialController::SerializeString(std::string s)
{
    std::ostringstream stream;
    stream << (unsigned char)2;
    char *string_length = LongerLength(s.size() + 12);
    stream << (unsigned char)string_length[0];
    stream << (unsigned char)string_length[1];
    stream << s;
    return stream.str();
}

std::string TerriBull::SerialController::DeserializeString(char *array, int *si)
{
    std::ostringstream stream;
    int length = (array[*si] << 8) + array[(*si) + 1];
    (*si) += 2;
    for (int i = *si; i < *si + length; i++)
    {
        stream << array[i];
    }
    (*si) += length;
    return stream.str();
}

void TerriBull::SerialController::ExchangeTags()
{
    while (!this->tagExchange)
    {
        if (!this->isCollectingTags)
        {
            for (auto it = this->Callbacks.begin(); it != this->Callbacks.end(); ++it)
            {
                CallbackItem *item = it->second;
                if (item->callback != nullptr)
                {
                    std::stringstream s3;
                    s3 << (char)1;
                    s3 << SerialController::SerializeNumber(this->Callbacks.size() - 1 == it->first ? 0 : 1);
                    s3 << SerialController::SerializeNumber(it->first);
                    s3 << SerialController::SerializeString(item->friendly_name);
                    this->SendData(s3.str());
                }
            }
        }
        this->ReadBuffer();
        pros::delay(5);
    }
}

int TerriBull::SerialController::RegisterCallback(std::string tag_name, PacketCallback callback)
{
    bool found = false;
    int returnIndex = -1;
    for (auto it = this->Callbacks.begin(); it != this->Callbacks.end(); ++it)
    {
        CallbackItem *item = it->second;
        if (item->friendly_name == tag_name)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        CallbackItem *item = new CallbackItem();
        item->friendly_name = tag_name;
        item->callback = callback;
        item->jetson_id = -1;
        returnIndex = this->Callbacks.size();
        this->Callbacks[returnIndex] = item;
    }
    else
    {
        throw new exception;
    }
    return returnIndex + SerialController::__packet_index_offset;
}

void TerriBull::SerialController::DeserializePacket()
{
    int func_id = __next_packet[__header_length];
    int pSize = __next_packet.size() - __header_length;
    int end = pSize - __footer_length;
    char buffer[end];
    memcpy(buffer, &__next_packet[__header_length + 1], end);

    if (func_id == 1)
    {
        this->ProcessTagExchange(buffer, 0, end);
    }
    else if (this->tagExchange && !this->isCollectingTags)
    {
        auto itr = Callbacks.find(func_id - SerialController::__packet_index_offset);
        if (itr == Callbacks.end())
            return;
        itr->second->callback(this->motherSys, buffer, 0, end);
    }
}

/**
 * @brief
 * DATA: [float step, int tag_id, string friendly_name]
 * @param array
 * @param start_index
 * @param length
 */
void TerriBull::SerialController::ProcessTagExchange(char *array, int start_index, int length)
{
    this->isCollectingTags = true;
    int ind = start_index + 1;

    int step = (int)SerialController::DeserializeNumber(array, &ind);

    ind++;
    int tag_id = (int)SerialController::DeserializeNumber(array, &ind);

    ind++;
    std::string friendly_name = SerialController::DeserializeString(array, &ind);

    int our_tag_id = this->GetCallbackIndex(friendly_name) - SerialController::__packet_index_offset;

    SerialController::CallbackItem *item;
    auto ret = this->Callbacks.find(our_tag_id);

    if (ret != this->Callbacks.end())
    {
        item = ret->second;
        item->jetson_id = tag_id;
    }
    else
    {
        item = new SerialController::CallbackItem();
        item->jetson_id = tag_id;
        item->friendly_name = friendly_name;
        this->Callbacks[Callbacks.size()] = item;
    }

    if (step == 0)
    {
        this->isCollectingTags = false;
        this->tagExchange = true;
    }
}

int TerriBull::SerialController::GetCallbackIndex(std::string tag_name)
{
    for (auto it = this->Callbacks.begin(); it != this->Callbacks.end(); ++it)
    {
        SerialController::CallbackItem *item = it->second;
        if (item->friendly_name == tag_name)
        {
            if (item->jetson_id == -1) /* We have not initialized the Packet index*/
                return it->first + SerialController::__packet_index_offset;
            else /* Packet Callback has been initialized on the Jetson */
                return item->jetson_id + SerialController::__packet_index_offset;
        }
    }
    return this->Callbacks.size() + SerialController::__packet_index_offset;
}

void TerriBull::SerialController::SendData(std::string data)
{
    for (auto c : SerialController::__packet_header)
    {
        std::cout << c;
    }
    std::cout << data;
    for (auto c : SerialController::__end_of_transmission)
    {
        std::cout << c;
    }

    // std::cout << (char)0 << (char)0 << (char)10 << (char)10;
    // std::cout << SerialController::__end_of_transmission;
}